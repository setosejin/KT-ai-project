#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import generator_stop, print_function

# import MicrophoneStream as MS
import asyncio
import http.client as httplib
import os
import sys
import time
from pathlib import Path

import RPi.GPIO as GPIO

# this file called from /etc/xdg/lxsession/LXDE-pi/autostart

DEBUG = False
loop = None

# 부팅시 오토런 실행할 지 여부
# 부팅할 때 오토런이 실행되면 콘솔모드가 종료되어
# PC에서 연결할 방법이 없다. 그래서 사용안함, 항상 False
ENABLE_ON_BOOT_AUTO_RUN = False

# 길게 누르기를 체크하는 시간
# 이 시간 이상은 체크할 필요가 없다.
# 예를 들어 누르고 있는 시간이 10초 이상인 경우는 시간을 추적할 필요가 없다
VERY_LONG_PRESS_SEC = 10

# 콘솔 모드를 체크하는 시간
# 예를 들어 3초간 누르고 있으면 콘솔모드로 전환
CONSOLE_TRIGGER_SEC = 3

# 누르고 있는 상태를 체크하는 간격(초)
PRESSING_CHECK_INTERVAL_SEC = 1.0

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(29, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(31, GPIO.OUT)

# key_path = '/home/pi/blockcoding/kt_ai_makers_kit_block_coding_driver/blockDriver/key/codingPackKey.json'
AUTO_RUN_FILE = '/home/pi/autorun/url.txt'
AUTO_RUN_CMD = '/home/pi/blockcoding/kt_ai_makers_kit_block_coding_driver/blockDriver/runBlock.sh'


def read_auto_run_url(filepath):
    p = Path(filepath)
    if not p.exists():
        return None
    contents = p.read_text()
    if contents is None:
        return None
    lines = [x for x in contents.splitlines(keepends=False) if
             x.startswith('https://') or x.startswith('http://')]
    if len(lines) > 0:
        return lines[0]
    return None


# 소리 재생, 재생중 현재 프로세스가 멈춘다
def play_audio(filename):
    # MS.play_file("/home/pi/autorun/py_script/data/" + filename)
    fpath = "/home/pi/autorun/py_script/data/" + filename
    os.system("aplay -q " + fpath)


# 소리 재생, 백그라운드 재생
def play_audio_background(filename):
    fpath = "/home/pi/autorun/py_script/data/" + filename
    os.system("aplay -q " + fpath + " &")


# 현재 시간, 밀리초
def current_milli():
    return round(time.time() * 1000)


# 인터넷 가능 여부 체크
def internet_available():
    conn = httplib.HTTPConnection("www.google.com", timeout=5)
    try:
        conn.request("HEAD", "/")
        return True
    except:
        return False
    finally:
        conn.close()


class ButtonGestureDetector:
    def __init__(self):
        self._last_press_down_at = 0
        self.on_press_up_listener = None

    def current_press_time_milli(self):
        if self._last_press_down_at == 0:
            return -1
        return current_milli() - self._last_press_down_at

    # 버튼을 눌렀을 때 호출
    def _on_press_down(self, pin):
        self._last_press_down_at = current_milli()

    # 버튼에서 손을 뗐을때 호출
    def _on_press_up(self, pin):
        if self._last_press_down_at == 0:
            if DEBUG:
                print('ignore pressUp, not ready')
            return
        diff = current_milli() - self._last_press_down_at
        self._last_press_down_at = 0
        if self.on_press_up_listener is not None:
            self.on_press_up_listener(diff)

    # GPIO 변경 핸들러
    def on_change_gpio(self, pin):
        if loop is None:
            return
        value = GPIO.input(pin)
        if DEBUG:
            print('on_change_gpio:', pin, value)
        if value == 0:
            self._on_press_down(pin)
        else:
            self._on_press_up(pin)


# 버튼 실행 핸들러
class AutoRunHandler(object):
    def __init__(self):
        print("__init__ AutoRunHandler")

    def execute(self):
        # 오토런 파일이 없습니다
        autorun_url = read_auto_run_url(AUTO_RUN_FILE)
        if autorun_url is None:
            print("No key...")
            GPIO.output(31, True)
            play_audio('no_autorun_project.wav')
            GPIO.output(31, False)
            return

        if not internet_available():
            GPIO.output(31, True)
            print("Network not Connected...")
            play_audio('no_wifi.wav')
            GPIO.output(31, False)
            return

        GPIO.output(31, True)
        print("Start AI Coding Block Button Execution ...")
        play_audio('bc_script.wav')  # 버튼 실행을 시작합니다
        GPIO.output(31, False)
        cmdline = ['lxterminal', '-e', AUTO_RUN_CMD + ' ' + autorun_url]
        os.system(' '.join(cmdline) + ' &')


# n초 동안 누르고 있으면 콘솔모드를 토글한다.
# n = CONSOLE_TRIGGER_SEC
class ConsoleButtonHandler:
    def __init__(self):
        self._is_handled = False
        self._trigger_when = CONSOLE_TRIGGER_SEC

    def handle_press_down(self, pressing_sec):
        if pressing_sec < self._trigger_when:
            self._is_handled = False
            return False  # not handled

        if not self._is_handled:
            self._is_handled = True
            self._run()
            return True  # handled

        return False  # not handled

    def _run(self):
        os.system('/usr/local/bin/aimk-button-serial-console.sh toggle &')


autorun_handler = AutoRunHandler()


#
# 아래 함수는 제거할 예정
#
# 프로그램이 시작될 때 호출되는 함수
# 부팅할 때 오토런이 실행되면 콘솔모드가 종료되어
# PC에서 연결할 방법이 없다. 그래서 사용안함
def on_booting():
    # 부팅할때 콘솔모드를 시작할지 체크해야 하는데
    # udev에 의해 자동으로 aimk-button-serial-console.sh가 실행되므로 여기서는 호출안함
    # os.system('/usr/local/bin/aimk-button-serial-console.sh check')

    autorun_url = read_auto_run_url(AUTO_RUN_FILE)
    if autorun_url is None:
        return
    autorun_handler.execute()


# 버튼에서 손을 땐 경우에 호출
# 2초 보다 적은 경우에만 기존 로직을 실행한다
def handle_press_up(pressed_time_milli):
    if loop is None:
        return
    if pressed_time_milli < 50:
        # 너무 짧게 터치한 경우다
        print('skip too short press up:', pressed_time_milli)
    elif pressed_time_milli < 2000:
        autorun_handler.execute()


async def main():
    console_handler = ConsoleButtonHandler()
    try:
        GPIO.output(31, False)
        gesture_detector = ButtonGestureDetector()
        # 버튼에서 손을 뗐을때 호출되는 함수 등록
        gesture_detector.on_press_up_listener = handle_press_up
        GPIO.add_event_detect(
            29, GPIO.BOTH, callback=gesture_detector.on_change_gpio, bouncetime=10)

        # 부팅시 오토런
        if ENABLE_ON_BOOT_AUTO_RUN:
            if not os.path.isfile('/tmp/aimk/.button_handler_on_boot'):
                os.system("sudo mkdir -p /tmp/aimk && sudo touch /tmp/aimk/.button_handler_on_boot")
                await asyncio.sleep(3)
                on_booting()

        # 버튼을 계속 누르는 상태를 체크한다
        # n초 동안 누르고 있으면 콘솔 모드를 토글한다
        # 주의: sleep(1초 간격)으로 체크하므로 시간이 정확하게 체크되지는 않는다.
        # 주의: 또 핸들링 함수를 실행하는 동안 시간을 잃어버려서 정확한 시간 체크는 불가능
        # 다른 체크를 추가하려면 ConsoleButtonHandler 같은 것을 하나 더 더 만드는게 좋다.
        while loop is not None:
            pressing_sec = gesture_detector.current_press_time_milli() // 1000
            if pressing_sec > VERY_LONG_PRESS_SEC:
                # 너무 오래 누르고 있는 상태, 조금 더 쉬어 버리자
                await asyncio.sleep(PRESSING_CHECK_INTERVAL_SEC * 1.5)
                continue
            console_handler.handle_press_down(pressing_sec)
            await asyncio.sleep(PRESSING_CHECK_INTERVAL_SEC)
    finally:
        GPIO.cleanup()


if __name__ == '__main__':
    try:
        loop = asyncio.get_event_loop()
        loop.run_until_complete(main())
        # loop.run_forever()
        loop.close()
    except:
        print("Error:", sys.exc_info()[0])
    finally:
        pass
