#!/usr/bin/env python
# -*- coding: utf_8 -*-
"""
 Modbus TestKit: Implementation of Modbus protocol in python

 (C)2009 - Luc Jean - luc.jean@gmail.com
 (C)2009 - Apidev - http://www.apidev.fr

 This is distributed under GNU LGPL license, see license.txt
"""
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
window = None
hooks_installed = False
led_widgets = {}
activity_until = {"rx": 0.0, "tx": 0.0}
activity_lock = threading.Lock()

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
#def list_ports():
def list_ports():
    ports = serial.tools.list_ports.comports()
    return [port.device for port in ports]

def open_port(port):
    try:
        global server, slave_1, hooks_installed
        server = modbus_rtu.RtuServer(serial.Serial(port))
        server.set_timeout(0.03)
        server.start()
        
        modbus_thread = threading.Thread(target=modbus_server_thread)
        modbus_thread.daemon = True
        modbus_thread.start()

        print(f"Opened port {port}")
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
        server = None
        slave_1 = None
        refresh_comm_leds()
        print(f"Failed to open port {port}: {e}")
        messagebox.showerror("Error", f"Failed to open port {port}: {e}")


def close_port():
    try:
        global server, slave_1
        if server :
            server.stop()
            server = None
            slave_1 = None
            print(f"Closed port")
            refresh_comm_leds()
            messagebox.showinfo("Success", "Closed port")
    except Exception as e:
        print(f"Failed to close port: {e}")
        refresh_comm_leds()

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
    tk.Label(cell, text=caption, bg=bg).pack(side=tk.LEFT, padx=(4, 0))


def on_write_request_multi(data):
       slave, pdu = data
       print(f"PDU={pdu}")
       print(pdu[0],pdu[1],pdu[2],pdu[3],pdu[4])
       address = pdu[1] << 8 |  pdu[2]
       count = pdu[3] << 8 |  pdu[4]
       byteCount = pdu[5] 
       modData= pdu[6] << 8 |  pdu[7]
       print(address,modData) 
       if address <59 : 
            modBusData[address] =  modData 
            entries[address].delete(0,tk.END)
            entries[address].insert(0, modData)
            slave_1.set_values('1',address,modData)
            slave_1.set_values('2',address,modData)
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
       if address <59 : 
            modBusData[address] =  modData 
            entries[address].delete(0,tk.END)
            entries[address].insert(0, modData)
            slave_1.set_values('1',address,modData)
            slave_1.set_values('2',address,modData)
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
    
    slave_1.set_values('1',15,int(setText_box.get()))
    slave_1.set_values('2',15,int(setText_box.get()))
    #print("Status Value ", int(setText_box.get()))

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
    slave_1.set_values('1',17,int(alarmStatusTextBox.get()))
    slave_1.set_values('2',17,int(alarmStatusTextBox.get()))

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
    slave_1.set_values('1',16,int(hwStatusTextBox.get()))
    slave_1.set_values('2',16,int(hwStatusTextBox.get()))
def entry_changed(event,index):
    global slave_1  # 전역 변수 사용
    print("index ", index)
    new_value = int(entries[index].get())

    # for i in range(len(modBusData)):
    #     modBusData[i] = modBusData[i] & 0xFFFF

    if slave_1 is not None:
        modBusData[index]= new_value
        #slave_1.set_values('1',0,list(range(200)))
        # slave_1.set_values('1', 0, modBusData)
        # slave_1.set_values('2', 0, modBusData)
        slave_1.set_values('2',index,new_value)
        slave_1.set_values('1',index,new_value)
        print("Entry", index, "changed to:", new_value)
        #register_values = slave_1.get_values('1', 0, 200)
        #register_values = modBusData; 
        # 레지스터 값 출력
        # print("Register values:")
        # for i, value in enumerate(register_values):
        #     print(f"Register {i}: {value}")
def on_closing():
    close_port()
    window.destroy()

def create_window():
    """main"""
    global toggled_states
    global slave_1
    toggled_states = {}
    global window
    window = tk.Tk()
    window.title("Modbus RTU 서버 제어")

    window.geometry("1350x830+0+0")
    window.config(bg="gray")

    portPanel = tk.Frame(window)
    portPanel.pack(side=tk.TOP,anchor='w', pady=10)

    port_label = tk.Label(portPanel,text="Select COM Port:")
    port_label.pack(side=tk.LEFT,padx=5)

    availabel_ports = list_ports()
    port_var = tk.StringVar(window)

    port_combobox = ttk.Combobox(portPanel,textvariable=port_var)
    port_combobox['values'] = availabel_ports
    port_combobox.pack(side=tk.LEFT,padx=5)

    open_button = tk.Button(portPanel,text="Open Port",command=lambda: open_port(port_var.get()))
    open_button.pack(side=tk.LEFT,padx=5)
    
    close_button = tk.Button(portPanel,text="Close Por", command=close_port)
    close_button.pack(side=tk.LEFT,padx=5)
    add_status_led(portPanel, "conn", "연결", LED_CONN)
    add_status_led(portPanel, "rx", "RX", LED_RX)
    add_status_led(portPanel, "tx", "TX", LED_TX)

    # button_frame = Frame(window,width=800,height=800)
    # button_frame.grid(row=0,column=0,padx=10,pady=5)
    # btn_run_main = tk.Button(window, text="Run main", command=main)
    # btn_run_main.pack(pady=10)

    # #infoPanel.pack(side=tk.BOTTOM,fill=tk.X,expand=True, pady=0)
    # infoPanel.place(relx=0.01, rely=0.7, relwidth=0.9, relheight=0.2)
    setPanel = tk.Frame(window)
    setPanel.pack(side=tk.TOP,anchor='w',pady=10)

    hwStatusPanel = tk.Frame(window)
    hwStatusPanel.pack(side=tk.TOP,anchor='w',pady=10)

    alarmPanel = tk.Frame(window)
    alarmPanel.pack(side=tk.TOP,anchor='w',pady=10)

    entryPanel = tk.Frame(window)
    entryPanel.pack(side=tk.TOP,pady=10)
    # "15" 버튼 생성 및 패널에 배치
    set_label = tk.Label(setPanel, text="STATUS")
    set_label.pack(side=tk.LEFT,padx=20)

    btn_setCharteStatusValue15= tk.Button(setPanel, text="15", command=lambda:set_bit_value(15,btn_setCharteStatusValue15))
    btn_setCharteStatusValue15.pack(side=tk.LEFT,padx=5)

    btn_setCharteStatusValue14= tk.Button(setPanel, text="14", command=lambda:set_bit_value(14,btn_setCharteStatusValue14))
    btn_setCharteStatusValue14.pack(side=tk.LEFT,padx=5)

    btn_setCharteStatusValue13= tk.Button(setPanel, text="13", command=lambda:set_bit_value(13,btn_setCharteStatusValue13))
    btn_setCharteStatusValue13.pack(side=tk.LEFT,padx=5)

    btn_setCharteStatusValue12= tk.Button(setPanel, text="12", command=lambda:set_bit_value(12,btn_setCharteStatusValue12))
    btn_setCharteStatusValue12.pack(side=tk.LEFT,padx=5)

    btn_setCharteStatusValue11= tk.Button(setPanel, text="11", command=lambda:set_bit_value(11,btn_setCharteStatusValue11))
    btn_setCharteStatusValue11.pack(side=tk.LEFT,padx=5)

    btn_setCharteStatusValue10= tk.Button(setPanel, text="10", command=lambda:set_bit_value(10,btn_setCharteStatusValue10))
    btn_setCharteStatusValue10.pack(side=tk.LEFT,padx=5)

    btn_setCharteStatusValue9= tk.Button(setPanel, text="9", command=lambda:set_bit_value(9,btn_setCharteStatusValue9))
    btn_setCharteStatusValue9.pack(side=tk.LEFT,padx=5)

    btn_setCharteStatusValue8= tk.Button(setPanel, text="8", command=lambda:set_bit_value(8,btn_setCharteStatusValue8))
    btn_setCharteStatusValue8.pack(side=tk.LEFT,padx=5)

    btn_setCharteStatusValue7= tk.Button(setPanel, text="7", command=lambda:set_bit_value(7,btn_setCharteStatusValue7))
    btn_setCharteStatusValue7.pack(side=tk.LEFT,padx=5)

    btn_setCharteStatusValue6= tk.Button(setPanel, text="6", command=lambda:set_bit_value(6,btn_setCharteStatusValue6))
    btn_setCharteStatusValue6.pack(side=tk.LEFT,padx=5)

    btn_setCharteStatusValue5= tk.Button(setPanel, text="5", command=lambda:set_bit_value(5,btn_setCharteStatusValue5))
    btn_setCharteStatusValue5.pack(side=tk.LEFT,padx=5)

    btn_setCharteStatusValue4= tk.Button(setPanel, text="4", command=lambda:set_bit_value(4,btn_setCharteStatusValue4))
    btn_setCharteStatusValue4.pack(side=tk.LEFT,padx=5)

    btn_setCharteStatusValue3= tk.Button(setPanel, text="3", command=lambda:set_bit_value(3,btn_setCharteStatusValue3))
    btn_setCharteStatusValue3.pack(side=tk.LEFT,padx=5)

    btn_setCharteStatusValue2= tk.Button(setPanel, text="2", command=lambda:set_bit_value(2,btn_setCharteStatusValue2))
    btn_setCharteStatusValue2.pack(side=tk.LEFT,padx=5)

    btn_setCharteStatusValue1= tk.Button(setPanel, text="1", command=lambda:set_bit_value(1,btn_setCharteStatusValue1))
    btn_setCharteStatusValue1.pack(side=tk.LEFT,padx=5)

    btn_setCharteStatusValue0= tk.Button(setPanel, text="0", command=lambda:set_bit_value(0,btn_setCharteStatusValue0))
    btn_setCharteStatusValue0.pack(side=tk.LEFT,padx=5)

    

    clr_label = tk.Label(hwStatusPanel, text="HW Status")
    clr_label.pack(side=tk.LEFT,padx=20)
    btn_hwStatusValue15= tk.Button(hwStatusPanel, text="15", command=lambda:hw_status(15,btn_hwStatusValue15))
    btn_hwStatusValue15.pack(side=tk.LEFT,padx=5)

    btn_hwStatusValue14= tk.Button(hwStatusPanel, text="14", command=lambda:hw_status(14,btn_hwStatusValue14))
    btn_hwStatusValue14.pack(side=tk.LEFT,padx=5)

    btn_hwStatusValue13= tk.Button(hwStatusPanel, text="13", command=lambda:hw_status(13,btn_hwStatusValue13))
    btn_hwStatusValue13.pack(side=tk.LEFT,padx=5)

    btn_hwStatusValue12= tk.Button(hwStatusPanel, text="12", command=lambda:hw_status(12,btn_hwStatusValue12))
    btn_hwStatusValue12.pack(side=tk.LEFT,padx=5)

    btn_hwStatusValue11= tk.Button(hwStatusPanel, text="11", command=lambda:hw_status(11,btn_hwStatusValue11))
    btn_hwStatusValue11.pack(side=tk.LEFT,padx=5)

    btn_hwStatusValue10= tk.Button(hwStatusPanel, text="10", command=lambda:hw_status(10,btn_hwStatusValue10))
    btn_hwStatusValue10.pack(side=tk.LEFT,padx=5)

    btn_hwStatusValue9= tk.Button(hwStatusPanel, text="9", command=lambda:hw_status(9,btn_hwStatusValue9))
    btn_hwStatusValue9.pack(side=tk.LEFT,padx=5)

    btn_hwStatusValue8= tk.Button(hwStatusPanel, text="8", command=lambda:hw_status(8,btn_hwStatusValue8))
    btn_hwStatusValue8.pack(side=tk.LEFT,padx=5)

    btn_hwStatusValue7= tk.Button(hwStatusPanel, text="7", command=lambda:hw_status(7,btn_hwStatusValue7))
    btn_hwStatusValue7.pack(side=tk.LEFT,padx=5)

    btn_hwStatusValue6= tk.Button(hwStatusPanel, text="6", command=lambda:hw_status(6,btn_hwStatusValue6))
    btn_hwStatusValue6.pack(side=tk.LEFT,padx=5)

    btn_hwStatusValue5= tk.Button(hwStatusPanel, text="5", command=lambda:hw_status(5,btn_hwStatusValue5))
    btn_hwStatusValue5.pack(side=tk.LEFT,padx=5)

    btn_hwStatusValue4= tk.Button(hwStatusPanel, text="4", command=lambda:hw_status(4,btn_hwStatusValue4))
    btn_hwStatusValue4.pack(side=tk.LEFT,padx=5)

    btn_hwStatusValue3= tk.Button(hwStatusPanel, text="3", command=lambda:hw_status(3,btn_hwStatusValue3))
    btn_hwStatusValue3.pack(side=tk.LEFT,padx=5)

    btn_hwStatusValue2= tk.Button(hwStatusPanel, text="2", command=lambda:hw_status(2,btn_hwStatusValue2))
    btn_hwStatusValue2.pack(side=tk.LEFT,padx=5)

    btn_hwStatusValue1= tk.Button(hwStatusPanel, text="1", command=lambda:hw_status(1,btn_hwStatusValue1))
    btn_hwStatusValue1.pack(side=tk.LEFT,padx=5)

    btn_hwStatusValue0= tk.Button(hwStatusPanel, text="0", command=lambda:hw_status(0,btn_hwStatusValue0))
    btn_hwStatusValue0.pack(side=tk.LEFT,padx=5)

    alarm_label = tk.Label(alarmPanel, text="Alarm Status")
    alarm_label.pack(side=tk.LEFT,padx=20)
    btn_alarmStatusValue15= tk.Button(alarmPanel, text="15", command=lambda:alarm_status(15,btn_alarmStatusValue15))
    btn_alarmStatusValue15.pack(side=tk.LEFT,padx=5)

    btn_alarmStatusValue14= tk.Button(alarmPanel, text="14", command=lambda:alarm_status(14,btn_alarmStatusValue14))
    btn_alarmStatusValue14.pack(side=tk.LEFT,padx=5)

    btn_alarmStatusValue13= tk.Button(alarmPanel, text="13", command=lambda:alarm_status(13,btn_alarmStatusValue13))
    btn_alarmStatusValue13.pack(side=tk.LEFT,padx=5)

    btn_alarmStatusValue12= tk.Button(alarmPanel, text="12", command=lambda:alarm_status(12,btn_alarmStatusValue12))
    btn_alarmStatusValue12.pack(side=tk.LEFT,padx=5)

    btn_alarmStatusValue11= tk.Button(alarmPanel, text="11", command=lambda:alarm_status(11,btn_alarmStatusValue11))
    btn_alarmStatusValue11.pack(side=tk.LEFT,padx=5)

    btn_alarmStatusValue10= tk.Button(alarmPanel, text="10", command=lambda:alarm_status(10,btn_alarmStatusValue10))
    btn_alarmStatusValue10.pack(side=tk.LEFT,padx=5)

    btn_alarmStatusValue9= tk.Button(alarmPanel, text="9", command=lambda:alarm_status(9,btn_alarmStatusValue9))
    btn_alarmStatusValue9.pack(side=tk.LEFT,padx=5)

    btn_alarmStatusValue8= tk.Button(alarmPanel, text="8", command=lambda:alarm_status(8,btn_alarmStatusValue8))
    btn_alarmStatusValue8.pack(side=tk.LEFT,padx=5)

    btn_alarmStatusValue7= tk.Button(alarmPanel, text="7", command=lambda:alarm_status(7,btn_alarmStatusValue7))
    btn_alarmStatusValue7.pack(side=tk.LEFT,padx=5)

    btn_alarmStatusValue6= tk.Button(alarmPanel, text="6", command=lambda:alarm_status(6,btn_alarmStatusValue6))
    btn_alarmStatusValue6.pack(side=tk.LEFT,padx=5)

    btn_alarmStatusValue5= tk.Button(alarmPanel, text="5", command=lambda:alarm_status(5,btn_alarmStatusValue5))
    btn_alarmStatusValue5.pack(side=tk.LEFT,padx=5)

    btn_alarmStatusValue4= tk.Button(alarmPanel, text="4", command=lambda:alarm_status(4,btn_alarmStatusValue4))
    btn_alarmStatusValue4.pack(side=tk.LEFT,padx=5)

    btn_alarmStatusValue3= tk.Button(alarmPanel, text="3", command=lambda:alarm_status(3,btn_alarmStatusValue3))
    btn_alarmStatusValue3.pack(side=tk.LEFT,padx=5)

    btn_alarmStatusValue2= tk.Button(alarmPanel, text="2", command=lambda:alarm_status(2,btn_alarmStatusValue2))
    btn_alarmStatusValue2.pack(side=tk.LEFT,padx=5)

    btn_alarmStatusValue1= tk.Button(alarmPanel, text="1", command=lambda:alarm_status(1,btn_alarmStatusValue1))
    btn_alarmStatusValue1.pack(side=tk.LEFT,padx=5)

    btn_alarmStatusValue0= tk.Button(alarmPanel, text="0", command=lambda:alarm_status(0,btn_alarmStatusValue0))
    btn_alarmStatusValue0.pack(side=tk.LEFT,padx=5)

    global setText_box
    setText_box = tk.Entry(setPanel)
    setText_box.pack(side=tk.LEFT,padx=10) 

    global hwStatusTextBox
    hwStatusTextBox = tk.Entry(hwStatusPanel)
    hwStatusTextBox.pack(side=tk.LEFT,padx=10) 

    global alarmStatusTextBox
    alarmStatusTextBox = tk.Entry(alarmPanel)
    alarmStatusTextBox.pack(side=tk.LEFT,padx=10) 
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
        set_label_1.grid(row=i, column=0, padx=5, pady=2)
        entry_1 = tk.Entry(entryPanel)
        entry_1.grid(row=i, column=1, padx=5, pady=2)
        entry_1.bind("<Return>", lambda event, index=i: entry_changed(event, index))
        entry_1.insert(0, entries_values[i])
        entries.append(entry_1)

        # 두 번째 엔트리(entry)와 라벨(label)
        set_label_2 = tk.Label(entryPanel, text=entries_names[i+1])
        set_label_2.grid(row=i, column=2, padx=5, pady=2)
        entry_2 = tk.Entry(entryPanel)
        entry_2.grid(row=i, column=3, padx=5, pady=2)
        entry_2.bind("<Return>", lambda event, index=i+1: entry_changed(event, index))
        entry_2.insert(0, entries_values[i+1])
        entries.append(entry_2)

        # 세 번째 엔트리(entry)와 라벨(label)
        set_label_3 = tk.Label(entryPanel, text=entries_names[i+2])
        set_label_3.grid(row=i, column=4, padx=5, pady=2)
        entry_3 = tk.Entry(entryPanel)
        entry_3.grid(row=i, column=5, padx=5, pady=2)
        entry_3.bind("<Return>", lambda event, index=i+2: entry_changed(event, index))
        entry_3.insert(0, entries_values[i+2])
        entries.append(entry_3)

        # 네 번째 엔트리(entry)와 라벨(label)
        set_label_4 = tk.Label(entryPanel, text=entries_names[i+3])
        set_label_4.grid(row=i, column=6, padx=5, pady=2)
        entry_4 = tk.Entry(entryPanel)
        entry_4.grid(row=i, column=7, padx=5, pady=2)
        entry_4.bind("<Return>", lambda event, index=i+3: entry_changed(event, index))
        entry_4.insert(0, entries_values[i+3])
        entries.append(entry_4)

        # 인덱스 조정
        # i = i + 3

    infoPanel = tk.Frame(window)
    #infoPanel.pack(side=tk.BOTTOM,fill=tk.X,expand=True, pady=0)
    infoPanel.place(relx=0.01, rely=0.7, relwidth=0.9, relheight=0.2)

    text="H/W Status info \n\n \
16.0	Input_OC(H/W Latch) 16.1	Inverter OC(H/W Latch) 16.2	Vdc_OV(H/W Latch)\n\n \
16.3	CONVERTER RUN/STOP STATE 16.4	DC/DC CONVERTER RUN/STOP STATE 16.5	Conv_GDU 16.6	Inv_GDU 16.7	GDU_DCDC \n\n\
16.8	Com_GDU \ 16.9	INVERTER RUN/STOP STATE \ 16.10	BAT_FUSE \ 16.11	Module OT \n\n\
16.12	Buzz ON/OFF Control from COM.  \ 16.13	EEPROM ERR \ 16.14	BAT MCCB Fault \ 16.15	TRANSFER RUN/STOP STATE\n"
    infoState_label = tk.Label(infoPanel,text=text ,justify='left',anchor='w',wraplength=1600)
    infoState_label.pack(side=tk.LEFT,padx=20)
    window.protocol("WM_DELETE_WINDOW", on_closing)
    window.after(LED_TICK_MS, tick_comm_leds)
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