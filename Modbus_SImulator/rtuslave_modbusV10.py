#!/usr/bin/env python
# -*- coding: utf_8 -*-
"""
 Modbus TestKit: Implementation of Modbus protocol in python

 (C)2009 - Luc Jean - luc.jean@gmail.com
 (C)2009 - Apidev - http://www.apidev.fr

 This is distributed under GNU LGPL license, see license.txt
"""
import atexit
import json
import os
import re
import threading
import random
import sys

import modbus_tk
import modbus_tk.hooks as hooks
import modbus_tk.defines as cst
from modbus_tk import modbus_rtu
import serial
from serial.tools import list_ports
import tkinter as tk
from tkinter import ttk,messagebox
import time
import numpy as np

import ctypes
from ctypes import c_uint


PORT = 'COM25'
entries = []  # Entry 위젯들을 저장할 리스트
LED_OFF = "#3d3d3d"
LED_CONN = "#22c55e"
LED_RX = "#facc15"
LED_TX = "#ef4444"
LED_PULSE_S = 0.12
LED_TICK_MS = 40
server = None
slave_1 = None
serial_port = None
window = None
hooks_installed = False
led_widgets = {}
activity_until = {"rx": 0.0, "tx": 0.0}
activity_lock = threading.Lock()
state_lock = threading.Lock()
STATE_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "simulator_state.json")
_GEOM_RE = re.compile(r"(\d+)x(\d+)([+-]\d+)([+-]\d+)")
selected_port = ""
port_should_reopen = False
port_var = None
last_geometry = "1580x900+0+0"
status_buttons = {}
hw_buttons = {}
alarm_buttons = {}

# data = []
# # 200개의 랜덤 값 생성
# for i in range(200):
#     data.append(random.randint(0, 200))
modBusData =  [0]*120
entries_values= [
#   "Nominal_Capacity", "Nominal_InputVoltage", "Nominal_OutputVoltage", "Nominal_BatVoltage",    "reserved_1",
  30, 220, 221, 222,0,
#   "reserved_2", "upsRun_t upsRun", "reserved_3", "Bat_Current_Ref", "Bat_Voltage_Ref", 
  0, 0, 0, 23, 233, 
#   "Output_Voltage_Ref", "HF_MODE", "converterStatus_t reserved_4", "reserved_5", "reserved_6",
  225, 0, 0, 0, 0, 
#   "ModuleState_t ModuleState",   "HWState_t HWState", "UpsOperationFault_t upsOperationFault", "reserved_7", "reserved_8", 
  0, 0, 0, 0, 0, 
#   "Input_volt_rms", "Input_current_rms", "vdc_link_volt_rms", "bat_volt_rms", "bat_current_rms",
  220, 18, 380, 238, 5, 
#   "inverter_volt_rms", "inverter_current_rms", "output_volt_rms", #   "output_current_rms", "conv_Frequency",
  221, 15, 221, 16, 599, 
#   "inv_Frequency", "bypass_Frequency", "battery_capacity", "load_percentage", "inv_internal_Temperature",
  0, 0, 93, 55, 0,
#   "reserved_9", "reserved_10", "reserved_11", "input_volt_gain", "input_current_gain",
  0, 0, 0, 223, 224, 
#   "vdc_link_volt_gain", "vbat_volt_gain", "bat_current_gain", "inverter_volt_gain", "inverter_current_gain", 
  225, 226,227, 228, 229, 
#   "GND_1", "output_current_gain", "reserved_12", "reserved_13", "reserved_14", 
  0, 230, 0, 0, 0, 
#   "input_volt_offset", "input_current_offset","vdc_link_volt_offset", "bat_volt_offset", "bat_current_offset",
  251, 252, 253, 254, 255, 
#   "inverter_volt_offset", "inverter_current_offset", "GND_2","output_current_offset" ,""
  256, 257, 258, 0, 250
  ]
# def on_write_request(address, value):
#     print(f"Received write request for address {address} with value {value}")
# def on_write_request_multi(slave, function_code, address, values):
#     print(f"Multiple write request - Slave: {slave}, Function Code: {function_code}, Address: {address}, Values: {values}")
def virtual_screen():
    if sys.platform == "win32":
        user32 = ctypes.windll.user32
        vx = user32.GetSystemMetrics(76)
        vy = user32.GetSystemMetrics(77)
        vw = user32.GetSystemMetrics(78)
        vh = user32.GetSystemMetrics(79)
        if vw > 0 and vh > 0:
            return vx, vy, vw, vh
    return 0, 0, 1920, 1080


def fit_geometry(spec):
    """저장된 좌표가 현재 화면 밖이면 보이는 영역 안으로 당긴다."""
    match = _GEOM_RE.search(spec or "")
    if not match:
        return "1580x900+0+0"
    w, h = int(match.group(1)), int(match.group(2))
    x, y = int(match.group(3)), int(match.group(4))
    vx, vy, vw, vh = virtual_screen()
    w = min(max(w, 400), vw)
    h = min(max(h, 300), vh)
    if x >= vx + vw or y >= vy + vh or x + w <= vx or y + h <= vy:
        x, y = vx, vy
    if x < vx:
        x = vx
    if y < vy:
        y = vy
    if x + w > vx + vw:
        x = vx + vw - w
    if y + h > vy + vh:
        y = vy + vh - h
    return f"{w}x{h}{x:+d}{y:+d}"


def current_port():
    if threading.current_thread() is threading.main_thread() and port_var is not None:
        try:
            text = port_var.get().strip()
            if text:
                return text
        except tk.TclError:
            pass
    return selected_port


def load_state():
    global selected_port, port_should_reopen, last_geometry
    if not os.path.exists(STATE_PATH):
        return
    try:
        with open(STATE_PATH, encoding="utf-8") as handle:
            data = json.load(handle)
    except (OSError, json.JSONDecodeError, TypeError):
        return
    values = data.get("values")
    if isinstance(values, list):
        for index, raw in enumerate(values):
            if index >= len(entries_values):
                break
            try:
                entries_values[index] = int(raw) & 0xFFFF
            except (TypeError, ValueError):
                pass
    selected_port = str(data.get("port") or "")
    port_should_reopen = bool(data.get("port_open"))
    if data.get("geometry"):
        last_geometry = str(data["geometry"])


def save_state():
    payload = {
        "port": current_port(),
        "port_open": bool(port_should_reopen),
        "geometry": last_geometry,
        "values": [int(v) & 0xFFFF for v in entries_values],
    }
    try:
        with state_lock:
            temporary = STATE_PATH + ".tmp"
            with open(temporary, "w", encoding="utf-8") as handle:
                json.dump(payload, handle, ensure_ascii=False, indent=2)
            os.replace(temporary, STATE_PATH)
    except OSError as exc:
        print(f"state save failed: {exc}")


_geom_after = None


def note_geometry(_event=None):
    global last_geometry, _geom_after
    if window is None:
        return
    if _event is not None and _event.widget is not window:
        return
    try:
        spec = window.geometry()
    except tk.TclError:
        return
    match = _GEOM_RE.search(spec)
    if not match:
        return
    if int(match.group(1)) < 200 or int(match.group(2)) < 200:
        return
    last_geometry = spec
    if _geom_after is not None:
        window.after_cancel(_geom_after)
    _geom_after = window.after(400, save_state)


def paint_word_buttons(buttons, value):
    for bit, btn in buttons.items():
        on = bool(int(value) & (1 << bit))
        toggled_states[btn] = on
        btn.config(background="red" if on else "SystemButtonFace")


def push_register(index, value):
    value = int(value) & 0xFFFF
    if index < len(entries_values):
        entries_values[index] = value
    if index < len(modBusData):
        modBusData[index] = value
    if index < len(entries):
        widget = entries[index]
        if widget.get() != str(value):
            widget.delete(0, tk.END)
            widget.insert(0, str(value))
    if slave_1 is not None:
        slave_1.set_values("1", index, value)
        slave_1.set_values("2", index, value)
    save_state()


def reflect_master_write(address, mod_data):
    if address < len(entries):
        entries[address].delete(0, tk.END)
        entries[address].insert(0, mod_data)
    if address == 15:
        global value_1
        value_1 = mod_data
        setText_box.delete(0, tk.END)
        setText_box.insert(0, str(mod_data))
        paint_word_buttons(status_buttons, mod_data)
    elif address == 16:
        global value_3
        value_3 = mod_data
        hwStatusTextBox.delete(0, tk.END)
        hwStatusTextBox.insert(0, str(mod_data))
        paint_word_buttons(hw_buttons, mod_data)
    elif address == 17:
        global value_2
        value_2 = mod_data
        alarmStatusTextBox.delete(0, tk.END)
        alarmStatusTextBox.insert(0, str(mod_data))
        paint_word_buttons(alarm_buttons, mod_data)
    save_state()


def on_master_write(address, mod_data):
    if not (0 <= address < 59):
        return
    modBusData[address] = mod_data
    if address < len(entries_values):
        entries_values[address] = mod_data
    if slave_1 is not None:
        slave_1.set_values("1", address, mod_data)
        slave_1.set_values("2", address, mod_data)
    if window is None:
        save_state()
        return
    try:
        window.after(0, lambda: reflect_master_write(address, mod_data))
    except tk.TclError:
        save_state()


#def list_ports():
def list_ports():
    ports = serial.tools.list_ports.comports()
    return [port.device for port in ports]

def open_port(port, quiet=False):
    try:
        global server, slave_1, serial_port, hooks_installed, selected_port, port_should_reopen
        if not port:
            return
        stop_server()
        serial_port = serial.Serial(port)
        server = modbus_rtu.RtuServer(serial_port)
        server.set_timeout(0.03)
        if server._thread is not None:
            server._thread.daemon = True
        server.start()
        
        modbus_thread = threading.Thread(target=modbus_server_thread)
        modbus_thread.daemon = True
        modbus_thread.start()

        print(f"Opened port {port}")
        selected_port = port
        port_should_reopen = True
        save_state()
        if not quiet:
            messagebox.showinfo("Success", f"Opened port {port}")
        
        # Add slave after opening the port
        slave_1 = server.add_slave(1)
        slave_1.add_block('1', cst.READ_HOLDING_REGISTERS, 0, 200)
        slave_1.set_values('1', 0, entries_values)
        slave_1.add_block('2', cst.READ_INPUT_REGISTERS, 0, 200)
        slave_1.set_values('2', 0, entries_values)
        
        hooks.install_hook("modbus.Slave.handle_write_single_register_request", on_write_request)
        hooks.install_hook("modbus.Slave.handle_write_multiple_registers_request", on_write_request_multi)
        if not hooks_installed:
            hooks.install_hook("modbus_rtu.RtuServer.after_read", on_rtu_rx)
            hooks.install_hook("modbus_rtu.RtuServer.before_write", on_rtu_tx)
            hooks_installed = True
        refresh_comm_leds()
        
    except Exception as e:
        stop_server()
        refresh_comm_leds()
        print(f"Failed to open port {port}: {e}")
        messagebox.showerror("Error", f"Failed to open port {port}: {e}")


def _release_serial(ser):
    if ser is None:
        return
    try:
        if ser.is_open:
            try:
                ser.cancel_read()
            except Exception:
                pass
            ser.close()
    except Exception as exc:
        print(f"Failed to close port: {exc}")


def stop_server():
    """서버 스레드를 끊고 COM 포트를 닫는다. 종료 후에도 포트가 잡혀 있지 않게 한다."""
    global server, slave_1, serial_port
    srv = server
    ser = serial_port if serial_port is not None else getattr(srv, "_serial", None)
    server = None
    slave_1 = None
    serial_port = None
    if srv is not None:
        try:
            srv._block_on_first_byte = False
            go = getattr(srv, "_go", None)
            if go is not None:
                go.clear()
        except Exception as exc:
            print(f"Failed to stop server: {exc}")
    if ser is not None:
        # 블로킹 read 가 예외로 풀린 뒤 라이브러리가 포트를 다시 열지 못하게 한다.
        ser.open = lambda *args, **kwargs: None
        try:
            if ser.is_open:
                ser.timeout = 0.05
        except Exception:
            pass
        _release_serial(ser)
    if srv is not None:
        thread = getattr(srv, "_thread", None)
        if thread is not None and thread.is_alive() and thread is not threading.current_thread():
            thread.join(timeout=1.0)
    _release_serial(ser)
    try:
        refresh_comm_leds()
    except Exception:
        pass


atexit.register(stop_server)


def close_port():
    global port_should_reopen
    was_open = server is not None
    port_should_reopen = False
    stop_server()
    save_state()
    if was_open:
        print("Closed port")
        messagebox.showinfo("Success", "Closed port")

def on_rtu_rx(_args):
    note_activity("rx")


def on_rtu_tx(args):
    _srv, response = args
    if response:
        note_activity("tx")


def note_activity(kind):
    with activity_lock:
        activity_until[kind] = time.monotonic() + LED_PULSE_S


def set_led(name, on, color_on):
    widget = led_widgets.get(name)
    if not widget:
        return
    canvas, oval = widget
    try:
        canvas.itemconfig(oval, fill=color_on if on else LED_OFF)
    except tk.TclError:
        pass


def refresh_comm_leds():
    now = time.monotonic()
    with activity_lock:
        rx_on = now < activity_until["rx"]
        tx_on = now < activity_until["tx"]
    connected = server is not None
    set_led("conn", connected, LED_CONN)
    set_led("rx", connected and rx_on, LED_RX)
    set_led("tx", connected and tx_on, LED_TX)


def tick_comm_leds():
    if window is None:
        return
    refresh_comm_leds()
    try:
        window.after(LED_TICK_MS, tick_comm_leds)
    except tk.TclError:
        pass


def add_status_led(parent, name, caption, _color_on):
    bg = parent.cget("bg") if str(parent.cget("bg")) else "gray"
    cell = tk.Frame(parent, bg=bg)
    cell.pack(side=tk.LEFT, padx=(10, 2))
    canvas = tk.Canvas(cell, width=16, height=16, bg=bg, highlightthickness=0, bd=0)
    canvas.pack(side=tk.LEFT)
    oval = canvas.create_oval(2, 2, 14, 14, fill=LED_OFF, outline="#1f1f1f", width=1)
    led_widgets[name] = (canvas, oval)
    tk.Label(cell, text=caption, bg=bg, fg="#f4f4f4").pack(side=tk.LEFT, padx=(4, 0))


def on_write_request_multi(data):
       slave, pdu = data
       print(f"PDU={pdu}")
       print(pdu[0],pdu[1],pdu[2],pdu[3],pdu[4])
       address = pdu[1] << 8 |  pdu[2]
       count = pdu[3] << 8 |  pdu[4]
       byteCount = pdu[5] 
       modData= pdu[6] << 8 |  pdu[7]
       print(address,modData) 
       on_master_write(address, modData)
       #slave_id = slave.id
       slave_id =   pdu[2]
       starting_address = 10 # pdu.starting_address
       written_value = 30 # pdu.written_value
       #response = modbus_tk.modbus.ModbusResponse(slave_id, cst.WRITE_SINGLE_REGISTER, starting_address, written_value)
       #slave_1.send_response(response)
def log_request_multi(args):
    slave, function_code, address, values = args
    print(f"Logging Multiple write request - Slave: {slave}, Function Code: {function_code}, Address: {address}, Values: {values}")

def on_write_request(data):
       slave, pdu = data
       print(f"PDU={pdu}")
       print(pdu[0],pdu[1],pdu[2],pdu[3],pdu[4])
       address = pdu[1] << 8 |  pdu[2]
       modData= pdu[3] << 8 |  pdu[4]
       print(address,modData) 
       on_master_write(address, modData)
       #slave_id = slave.id
       slave_id =   pdu[2]
       starting_address = 10 # pdu.starting_address
       written_value = 30 # pdu.written_value
       #response = modbus_tk.modbus.ModbusResponse(slave_id, cst.WRITE_SINGLE_REGISTER, starting_address, written_value)
       #slave_1.send_response(response)
       
def modbus_server_thread():
    global slave_1, server
    logger = modbus_tk.utils.create_logger(name="console", record_format="%(message)s")

    try:
        logger.info("running...")
        logger.info("enter 'quit' for closing the server")

        while True:
            time.sleep(0.5)
    except Exception as e:
        logger.error(f"Error in Modbus server thread: {e}")

# if __name__ == "__main__":
# main()
def main():
    global server
    # modbus_thread = threading.Thread(target=modbus_server_thread)

value_1 = 0 
value_2 = 0 
value_3 = 0 
#setButtonStatus =0
def set_bit_value(bit,button):
    #global setButtonStatus
    global value_1 
    global toggled_states
    toggled_states[button] = not toggled_states.get(button, False)
    if toggled_states[button]:
        button.config(background="red")  # 눌린 상태
        value_1 |=1 << bit
        setText_box.delete(0,tk.END)
        setText_box.insert(0,str(value_1))
    else:
        button.config(background="SystemButtonFace")  # 눌리지 않은 상태, 기본 배경색
        value_1 &=  ~(1 << bit)
        setText_box.delete(0,tk.END)
        setText_box.insert(0,str(value_1))
    push_register(15, value_1)

def alarm_status(bit,button):
    # global setButtonStatus
    global value_2 
    global toggled_states
    toggled_states[button] = not toggled_states.get(button, False)
    if toggled_states[button]:
        button.config(background="red")  # 눌린 상태
        value_2 |=1 << bit
        alarmStatusTextBox.delete(0,tk.END)
        alarmStatusTextBox.insert(0,str(value_2))
    else:
        button.config(background="SystemButtonFace")  # 눌리지 않은 상태, 기본 배경색
        value_2 &=  ~(1 << bit)
        alarmStatusTextBox.delete(0,tk.END)
        alarmStatusTextBox.insert(0,str(value_2))
    push_register(17, value_2)

def hw_status(bit,button):
    #global setButtonStatus
    global value_3 
    global toggled_states
    toggled_states[button] = not toggled_states.get(button, False)
    if toggled_states[button]:
        button.config(background="red")  # 눌린 상태
        value_3 |=1 << bit
        hwStatusTextBox.delete(0,tk.END)
        hwStatusTextBox.insert(0,str(value_3))
    else:
        button.config(background="SystemButtonFace")  # 눌리지 않은 상태, 기본 배경색
        value_3 &=  ~(1 << bit)
        hwStatusTextBox.delete(0,tk.END)
        hwStatusTextBox.insert(0,str(value_3))
    push_register(16, value_3)
def entry_changed(event,index):
    print("index ", index)
    try:
        new_value = int(entries[index].get()) & 0xFFFF
    except ValueError:
        return
    if index == 15:
        global value_1
        value_1 = new_value
        setText_box.delete(0, tk.END)
        setText_box.insert(0, str(new_value))
        paint_word_buttons(status_buttons, new_value)
    elif index == 16:
        global value_3
        value_3 = new_value
        hwStatusTextBox.delete(0, tk.END)
        hwStatusTextBox.insert(0, str(new_value))
        paint_word_buttons(hw_buttons, new_value)
    elif index == 17:
        global value_2
        value_2 = new_value
        alarmStatusTextBox.delete(0, tk.END)
        alarmStatusTextBox.insert(0, str(new_value))
        paint_word_buttons(alarm_buttons, new_value)
    push_register(index, new_value)
    print("Entry", index, "changed to:", new_value)
def on_closing():
    try:
        note_geometry()
        save_state()
    finally:
        stop_server()
        try:
            window.destroy()
        except tk.TclError:
            pass

# 단상 HMI 통신 엑셀 번지 15/16/17. 인덱스 = 비트 번호.
STATUS_BITS = [
    "충전\n운전", "충전\n재기동", "충전\n정지", "이상\n충전정지",
    "DCDC\n운전", "DCDC\n재기동", "DCDC\n정지", "이상\nDCDC정지",
    "INV\n운전", "INV\n재기동", "INV\n정지", "이상\nINV정지",
    "INV\n절환", "BYP\n절환", "이상\nBYP절환", "충방전",
]
HW_BITS = [
    "입력OC", "INV OC", "Vdc OV", "CONV\n운전",
    "DCDC\n운전", "CONV\nGDU", "INV\nGDU", "DCDC\nGDU",
    "N상\nGDU", "INV\n운전", "BAT\n퓨즈", "모듈OT",
    "부저", "EEPROM", "BAT\n차단기", "절환\nINV/BYP",
]
ALARM_BITS = [
    "충전\n전류제한", "DC\n과전압", "DC\n저전압", "입력\n저전압",
    "입력\n과전압", "입력\n주파수", "INV\n주파수", "정전",
    "BAT전류\n제한", "BAT\n과전압", "BAT\n저전압", "INV출력\n전압",
    "출력\n과부하", "INV과부하\n정지", "옵셋\n이상", "출력CT",
]


BIT_COL_W = 72
TITLE_W = 78
LEFT_PAD = 10
UI_BG = "gray"
UI_FG = "#f4f4f4"


def add_bit_row(parent, row, names, on_click):
    buttons = {}
    for bit in range(15, -1, -1):
        col = 1 + (15 - bit)
        cell = tk.Frame(parent, bg=UI_BG)
        cell.grid(row=row, column=col, sticky="n", pady=2)
        btn = tk.Button(cell, text=str(bit), width=3)
        btn.configure(command=lambda b=bit, bt=btn: on_click(b, bt))
        btn.pack(side=tk.TOP)
        tk.Label(
            cell,
            text=names[bit],
            font=("Malgun Gothic", 8),
            justify="center",
            bg=UI_BG,
            fg=UI_FG,
            wraplength=BIT_COL_W - 6,
        ).pack(side=tk.TOP)
        buttons[bit] = btn
    return buttons


def create_window():
    """main"""
    global toggled_states
    global slave_1
    global window
    global port_var
    global value_1, value_2, value_3
    global status_buttons, hw_buttons, alarm_buttons
    load_state()
    value_1 = int(entries_values[15]) & 0xFFFF
    value_3 = int(entries_values[16]) & 0xFFFF
    value_2 = int(entries_values[17]) & 0xFFFF
    toggled_states = {}
    window = tk.Tk()
    window.title("Modbus RTU 서버 제어")

    window.geometry(fit_geometry(last_geometry))
    window.config(bg="gray")
    window.bind("<Configure>", note_geometry)

    bitPanel = tk.Frame(window, bg=UI_BG)
    bitPanel.pack(side=tk.TOP, anchor="w", padx=LEFT_PAD, pady=(8, 0))
    bitPanel.grid_columnconfigure(0, minsize=TITLE_W)
    for col in range(1, 17):
        bitPanel.grid_columnconfigure(col, minsize=BIT_COL_W, uniform="bit")

    portPanel = tk.Frame(bitPanel, bg=UI_BG)
    portPanel.grid(row=0, column=0, columnspan=18, sticky="ew", pady=(0, 8))

    port_label = tk.Label(portPanel, text="Select COM Port:", bg=UI_BG, fg=UI_FG)
    port_label.pack(side=tk.LEFT, padx=(0, 5))

    availabel_ports = list_ports()
    port_var = tk.StringVar(window)
    if selected_port:
        port_var.set(selected_port)

    port_combobox = ttk.Combobox(portPanel, textvariable=port_var, width=12)
    port_combobox['values'] = availabel_ports
    port_combobox.pack(side=tk.LEFT, padx=5)

    open_button = tk.Button(portPanel, text="Open Port", command=lambda: open_port(port_var.get()))
    open_button.pack(side=tk.LEFT, padx=5)

    close_button = tk.Button(portPanel, text="Close Por", command=close_port)
    close_button.pack(side=tk.LEFT, padx=5)
    tk.Frame(portPanel, bg=UI_BG).pack(side=tk.LEFT, fill=tk.X, expand=True)
    add_status_led(portPanel, "conn", "연결", LED_CONN)
    add_status_led(portPanel, "rx", "RX", LED_RX)
    add_status_led(portPanel, "tx", "TX", LED_TX)

    # button_frame = Frame(window,width=800,height=800)
    # button_frame.grid(row=0,column=0,padx=10,pady=5)
    # btn_run_main = tk.Button(window, text="Run main", command=main)
    # btn_run_main.pack(pady=10)

    # #infoPanel.pack(side=tk.BOTTOM,fill=tk.X,expand=True, pady=0)
    # infoPanel.place(relx=0.01, rely=0.7, relwidth=0.9, relheight=0.2)
    entryPanel = tk.Frame(window)
    entryPanel.pack(side=tk.TOP, anchor="w", padx=LEFT_PAD, pady=10)

    def place_bit_title(row, text):
        tk.Label(
            bitPanel, text=text, bg=UI_BG, fg=UI_FG,
            justify="center", font=("Malgun Gothic", 9),
        ).grid(row=row, column=0, sticky="nw", padx=(0, 4))

    place_bit_title(1, "STATUS\n번지15")
    status_buttons = add_bit_row(bitPanel, 1, STATUS_BITS, set_bit_value)
    place_bit_title(2, "HW\n번지16")
    hw_buttons = add_bit_row(bitPanel, 2, HW_BITS, hw_status)
    place_bit_title(3, "Alarm\n번지17")
    alarm_buttons = add_bit_row(bitPanel, 3, ALARM_BITS, alarm_status)
    paint_word_buttons(status_buttons, value_1)
    paint_word_buttons(hw_buttons, value_3)
    paint_word_buttons(alarm_buttons, value_2)

    global setText_box
    setText_box = tk.Entry(bitPanel, width=8)
    setText_box.grid(row=1, column=17, sticky="n", padx=(8, 0))
    setText_box.insert(0, str(value_1))

    global hwStatusTextBox
    hwStatusTextBox = tk.Entry(bitPanel, width=8)
    hwStatusTextBox.grid(row=2, column=17, sticky="n", padx=(8, 0))
    hwStatusTextBox.insert(0, str(value_3))

    global alarmStatusTextBox
    alarmStatusTextBox = tk.Entry(bitPanel, width=8)
    alarmStatusTextBox.grid(row=3, column=17, sticky="n", padx=(8, 0))
    alarmStatusTextBox.insert(0, str(value_2)) 
    # canvas = tk.Canvas(window)
    # canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
    # scrollbar = tk.Scrollbar(window, orient="vertical", command=canvas.yview)
    # scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
    # canvas.configure(yscrollcommand=scrollbar.set)
    # canvas.bind('<Configure>', lambda e: canvas.configure(scrollregion=canvas.bbox("all")))

    entries_names= [
        "Nominal_Capacity", "Nominal_InputVoltage", "Nominal_OutputVoltage", "Nominal_BatVoltage", "reserved_1", 
        "reserved_2", "upsRun_t upsRun", "reserved_3", "Bat_Current_Ref", "Bat_Voltage_Ref",
        "Output_Voltage_Ref", "HF_MODE", "converterStatus_t reserved_4", "reserved_5", "reserved_6", 
        "ModuleState_t ModuleState", "HWState_t HWState", "UpsOperationFault_t upsOperationFault", "reserved_7", "reserved_8",
        "Input_volt_rms", "Input_current_rms", "vdc_link_volt_rms", "bat_volt_rms", "bat_current_rms", 
        "inverter_volt_rms", "inverter_current_rms", "output_volt_rms", "output_current_rms", "conv_Frequency", 
        "inv_Frequency", "bypass_Frequency", "battery_capacity", "load_percentage", "inv_internal_Temperature", 
        "reserved_9", "reserved_10", "reserved_11", "input_volt_gain", "input_current_gain", 
        "vdc_link_volt_gain", "vbat_volt_gain", "bat_current_gain", "inverter_volt_gain", "inverter_current_gain", 
        "GND_1", "output_current_gain", "reserved_12", "reserved_13", "reserved_14", 
        "input_volt_offset", "input_current_offset", "vdc_link_volt_offset", "bat_volt_offset", "bat_current_offset", 
        "inverter_volt_offset", "inverter_current_offset", "GND_2", "output_current_offset","" ]

    for i in range(0,58,4):
        # 첫 번째 엔트리(entry)와 라벨(label)
        set_label_1 = tk.Label(entryPanel, text=entries_names[i])
        set_label_1.grid(row=i, column=0, padx=(0, 5), pady=2, sticky="w")
        entry_1 = tk.Entry(entryPanel)
        entry_1.grid(row=i, column=1, padx=5, pady=2, sticky="w")
        entry_1.bind("<Return>", lambda event, index=i: entry_changed(event, index))
        entry_1.insert(0, entries_values[i])
        entries.append(entry_1)

        # 두 번째 엔트리(entry)와 라벨(label)
        set_label_2 = tk.Label(entryPanel, text=entries_names[i+1])
        set_label_2.grid(row=i, column=2, padx=5, pady=2, sticky="w")
        entry_2 = tk.Entry(entryPanel)
        entry_2.grid(row=i, column=3, padx=5, pady=2, sticky="w")
        entry_2.bind("<Return>", lambda event, index=i+1: entry_changed(event, index))
        entry_2.insert(0, entries_values[i+1])
        entries.append(entry_2)

        # 세 번째 엔트리(entry)와 라벨(label)
        set_label_3 = tk.Label(entryPanel, text=entries_names[i+2])
        set_label_3.grid(row=i, column=4, padx=5, pady=2, sticky="w")
        entry_3 = tk.Entry(entryPanel)
        entry_3.grid(row=i, column=5, padx=5, pady=2, sticky="w")
        entry_3.bind("<Return>", lambda event, index=i+2: entry_changed(event, index))
        entry_3.insert(0, entries_values[i+2])
        entries.append(entry_3)

        # 네 번째 엔트리(entry)와 라벨(label)
        set_label_4 = tk.Label(entryPanel, text=entries_names[i+3])
        set_label_4.grid(row=i, column=6, padx=5, pady=2, sticky="w")
        entry_4 = tk.Entry(entryPanel)
        entry_4.grid(row=i, column=7, padx=5, pady=2, sticky="w")
        entry_4.bind("<Return>", lambda event, index=i+3: entry_changed(event, index))
        entry_4.insert(0, entries_values[i+3])
        entries.append(entry_4)

        # 인덱스 조정
        # i = i + 3

    window.protocol("WM_DELETE_WINDOW", on_closing)
    window.after(LED_TICK_MS, tick_comm_leds)
    if port_should_reopen and selected_port in availabel_ports:
        window.after(300, lambda port=selected_port: open_port(port, quiet=True))
    window.mainloop()

if __name__ == "__main__":
    main()
    create_window()

    #window.maxsize(900,600)
    # window.config(bg="skyblue")
    # left_frame = Frame(window,width=200,height=400)
    # left_frame.grid(row=0,column=0,padx=10,pady=5)
    # tool_bar = Frame(left_frame,width=180,height=185,bg="purple")
    # tool_bar.grid(row=2,column=0,padx=5,pady=5)

    #Label(left_frame,text="Original Image").grid(row=1,column=0,padx=5,pady=5)