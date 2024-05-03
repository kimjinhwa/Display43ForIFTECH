## How To Manuplate Keyboard
- lv_obj_t *ui_txtTempory; 
- extern lv_obj_t *ui_txtTempory;
- lv_textarea_set_text(ui_txtTempory ,lv_textarea_get_text( ui_txtInputArea)) ; 
- void ui_event_txtBatCurrSet( lv_event_t * e) {
      ui_txtTempory = ui_txtBatCurrSet; 