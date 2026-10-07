#include "screen_capture.h"

#include <Arduino.h>
#include <lvgl.h>
#include <stdlib.h>
#include <string.h>

#include "SimpleCLI.h"
#include "myBlueTooth.h"
#include "esp_heap_caps.h"
#include "esp_task_wdt.h"

extern "C" {
void lv_obj_redraw(lv_draw_ctx_t *draw_ctx, lv_obj_t *obj);
void _lv_refr_set_disp_refreshing(lv_disp_t *disp);
lv_disp_t *_lv_refr_get_disp_refreshing(void);
}

/*
 * LVGL 드로우 버퍼는 화면의 1/6이라 그 메모리를 덤프하면 띠 하나만 나온다.
 * lv_snapshot 과 같이 현재 화면 트리를 RGB565로 다시 그리되, 전체 프레임을
 * 잡지 않고 가로 띠만 malloc 한다. 4.3"(PSRAM)과 3.2"(내부 RAM)가 같은 경로다.
 * 한 명령 안에서 띠를 이어 그리므로, 밴드 사이에 UI가 바뀌어 화면이 찢어지지 않는다.
 */

#if defined(SCREEN_CAPTURE)

static const char kB64[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static void *allocBand(size_t bytes)
{
    void *p = heap_caps_malloc(bytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (p == nullptr)
        p = heap_caps_malloc(bytes, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    if (p == nullptr)
        p = malloc(bytes);
    return p;
}

static void emitB64(const uint8_t *data, size_t len)
{
    char line[512];
    int col = 0;
    size_t i = 0;

    while (i + 3 <= len) {
        uint32_t v = ((uint32_t)data[i] << 16) | ((uint32_t)data[i + 1] << 8) | data[i + 2];
        line[col++] = kB64[(v >> 18) & 63];
        line[col++] = kB64[(v >> 12) & 63];
        line[col++] = kB64[(v >> 6) & 63];
        line[col++] = kB64[v & 63];
        i += 3;
        if (col >= 480) {
            line[col++] = '\r';
            line[col++] = '\n';
            mySerialBT.writePaced((const uint8_t *)line, (size_t)col);
            col = 0;
            (void)esp_task_wdt_reset();
        }
    }
    if (i < len) {
        uint8_t a = data[i];
        uint8_t b = (i + 1 < len) ? data[i + 1] : 0;
        uint32_t v = ((uint32_t)a << 16) | ((uint32_t)b << 8);
        line[col++] = kB64[(v >> 18) & 63];
        line[col++] = kB64[(v >> 12) & 63];
        if (i + 1 < len) {
            line[col++] = kB64[(v >> 6) & 63];
            line[col++] = '=';
        } else {
            line[col++] = '=';
            line[col++] = '=';
        }
    }
    if (col > 0) {
        line[col++] = '\r';
        line[col++] = '\n';
        mySerialBT.writePaced((const uint8_t *)line, (size_t)col);
    }
}

static bool renderBand(lv_obj_t *scr, lv_disp_t *disp, lv_draw_ctx_t *draw_ctx,
                       lv_color_t *buf, int y, int w, int bandH)
{
    lv_area_t area;
    area.x1 = 0;
    area.y1 = (lv_coord_t)y;
    area.x2 = (lv_coord_t)(w - 1);
    area.y2 = (lv_coord_t)(y + bandH - 1);

    draw_ctx->clip_area = &area;
    draw_ctx->buf_area = &area;
    draw_ctx->buf = buf;
    memset(buf, 0, (size_t)w * (size_t)bandH * sizeof(lv_color_t));

    lv_disp_t *prev = _lv_refr_get_disp_refreshing();
    _lv_refr_set_disp_refreshing(disp);
    lv_obj_redraw(draw_ctx, scr);
    _lv_refr_set_disp_refreshing(prev);
    return true;
}

static void scrCallback(cmd *cmdPtr)
{
    (void)cmdPtr;

    lv_obj_t *scr = lv_scr_act();
    lv_disp_t *objDisp = scr ? lv_obj_get_disp(scr) : nullptr;
    if (scr == nullptr || objDisp == nullptr || objDisp->driver == nullptr ||
        objDisp->driver->draw_ctx_init == nullptr) {
        mySerialBT.printf("\r\nSCR ERR nodisp\r\n");
        return;
    }

    int w = lv_obj_get_width(scr);
    int h = lv_obj_get_height(scr);
    if (w <= 0 || h <= 0) {
        mySerialBT.printf("\r\nSCR ERR nodisp\r\n");
        return;
    }

    static const int kTryLines[] = {16, 8, 4, 2, 1};
    int lines = 0;
    lv_color_t *buf = nullptr;
    for (int n : kTryLines) {
        buf = (lv_color_t *)allocBand((size_t)w * (size_t)n * sizeof(lv_color_t));
        if (buf != nullptr) {
            lines = n;
            break;
        }
    }
    if (buf == nullptr) {
        mySerialBT.printf("\r\nSCR ERR nomem\r\n");
        return;
    }

    lv_draw_ctx_t *drawCtx = (lv_draw_ctx_t *)malloc(objDisp->driver->draw_ctx_size);
    if (drawCtx == nullptr) {
        free(buf);
        mySerialBT.printf("\r\nSCR ERR nomem\r\n");
        return;
    }

    lv_disp_drv_t driver;
    lv_disp_drv_init(&driver);
    driver.hor_res = lv_disp_get_hor_res(objDisp);
    driver.ver_res = lv_disp_get_ver_res(objDisp);
    lv_disp_drv_use_generic_set_px_cb(&driver, LV_IMG_CF_TRUE_COLOR);

    lv_disp_t fake;
    lv_memset_00(&fake, sizeof(fake));
    fake.driver = &driver;

    objDisp->driver->draw_ctx_init(&driver, drawCtx);
    driver.draw_ctx = drawCtx;

    int bands = (h + lines - 1) / lines;
    mySerialBT.printf("\r\nSCR W=%d H=%d CF=RGB565 SWAP=%d LINES=%d BANDS=%d\r\n",
                      w, h, LV_COLOR_16_SWAP, lines, bands);

    for (int y = 0, index = 0; y < h; y += lines, index++) {
        int bandH = h - y;
        if (bandH > lines)
            bandH = lines;
        size_t nbytes = (size_t)w * (size_t)bandH * sizeof(lv_color_t);
        (void)esp_task_wdt_reset();
        renderBand(scr, &fake, drawCtx, buf, y, w, bandH);
        mySerialBT.printf("SCRB %d %d %d %u\r\n", index, y, bandH, (unsigned)nbytes);
        emitB64((const uint8_t *)buf, nbytes);
        mySerialBT.printf("SCRE %d\r\n", index);
        vTaskDelay(1);
    }

    if (objDisp->driver->draw_ctx_deinit)
        objDisp->driver->draw_ctx_deinit(&driver, drawCtx);
    mySerialBT.printf("SCR DONE\r\n");
    free(drawCtx);
    free(buf);
}

#endif /* SCREEN_CAPTURE */

void screenCaptureCliInit(void)
{
#if defined(SCREEN_CAPTURE)
    Command c = simpleCli.addCommand("scr", scrCallback);
    c.setDescription("Capture the current screen (RGB565, BLE)");
#else
    /* 배포에서 -DSCREEN_CAPTURE 를 빼면 scr 명령 자체가 없다. */
#endif
}
