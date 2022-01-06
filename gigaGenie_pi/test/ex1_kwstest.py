#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""Example 1: GiGA Genie Keyword Spotting"""

from __future__ import print_function

import audioop
from ctypes import *
import RPi.GPIO as GPIO
import ktkws # KWS
import MicrophoneStream as MS
import vlc
import threading
import time
import os
import sys
sys.path.append("../../motion/")
import main_openpose as mo
import subprocess

# 볼륨
VOLUME = 70

KWSID = ['기가지니', '지니야', '친구야', '자기야']
RATE = 16000
CHUNK = 512
wake_up = False

GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)
GPIO.setup(29, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(31, GPIO.OUT)
btn_status = False

def callback(channel):  
	print("falling edge detected from pin {}".format(channel))
	global btn_status
	btn_status = True
	print(btn_status)

GPIO.add_event_detect(29, GPIO.FALLING, callback=callback, bouncetime=10)

ERROR_HANDLER_FUNC = CFUNCTYPE(None, c_char_p, c_int, c_char_p, c_int, c_char_p)
def py_error_handler(filename, line, function, err, fmt):
  dummy_var = 0
c_error_handler = ERROR_HANDLER_FUNC(py_error_handler)
asound = cdll.LoadLibrary('libasound.so')
asound.snd_lib_error_set_handler(c_error_handler)


def detect():
	# stream generator를 이용한 기가지니 단어 인식
	response = vlc.MediaPlayer("response.mp3")
	response.audio_set_volume(VOLUME)
	with MS.MicrophoneStream(RATE, CHUNK) as stream:
		audio_generator = stream.generator()
	
		for content in audio_generator:
			rc = ktkws.detect(content)
			rms = audioop.rms(content,2)
			#print('audio rms = %d' % (rms))

			if (rc == 1):
				response.play()
				#MS.play_file("../data/sample_sound.wav")
				return 200

def Alarm(alarm, que):
	# alarm 울리는 메서드와 동시에 기지개 동작 감지하는 inference 함수 실행
	global wake_up

	print("played alarm")
	alarm.play()

	# alarm flag 전송
	alarm_cnt = 0
	while True:
		alarm_cnt += 1
		data = subprocess.check_output(['./send_alarm_flag'])
		if 'fail' not in data.decode()  : break
		if alarm_cnt > 10 : break
	
	# inference 함수 실행
	if mo.inference(que):
		alarm.stop()
		return


def btn_detect(standard_time, second, que):
	# 
	global btn_status

	alarm = vlc.MediaPlayer("long_alarm.mp3")
	alarm.audio_set_volume(VOLUME)
	alarm_thread = threading.Thread(target=Alarm, args=(alarm, que))
	trigger = True

	response = vlc.MediaPlayer("response.mp3")
	response.audio_set_volume(VOLUME)
	with MS.MicrophoneStream(RATE, CHUNK) as stream:
		audio_generator = stream.generator()

		for content in audio_generator:
			now = time.time()

			# 알람이 울릴 때 alarm 쓰레드 실행
			if ((now - standard_time) > second) and (standard_time > 0) and trigger: 
				alarm_thread.start()
				trigger = False
				return 300, alarm
			GPIO.output(31, GPIO.HIGH)
			rc = ktkws.detect(content)

			rms = audioop.rms(content,2)
			#print('audio rms = %d' % (rms))
			GPIO.output(31, GPIO.LOW)

			if (btn_status == True):
				rc = 2
				btn_status = False			
			
			# 기가지니 불리면 응답후 200 반환
			if (rc == 1):
				GPIO.output(31, GPIO.HIGH)
				response.play()
				#MS.play_file("../data/sample_sound.wav")
				return 200, alarm
				
			# 버튼 눌리면 100 반환
			elif (rc == 2):
				return 100, alarm


def test(key_word = '기가지니'):
	rc = ktkws.init("../data/kwsmodel.pack")
	print ('init rc = %d' % (rc))
	rc = ktkws.start()
	print ('start rc = %d' % (rc))
	print ('\n호출어를 불러보세요~\n')
	ktkws.set_keyword(KWSID.index(key_word))
	rc = detect()
	print ('detect rc = %d' % (rc))
	print ('\n\n호출어가 정상적으로 인식되었습니다.\n\n')
	ktkws.stop()
	return rc

def btn_test(standard_time, second, que, key_word = '기가지니'):
	global btn_status
	rc = ktkws.init("../data/kwsmodel.pack")
	print ('init rc = %d' % (rc))
	rc = ktkws.start()
	print ('start rc = %d' % (rc))
	print ('\n버튼을 눌러보세요~\n')
	ktkws.set_keyword(KWSID.index(key_word))
	rc, alarm = btn_detect(standard_time, second, que)
	print ('detect rc = %d' % (rc))
	print ('\n\n호출어가 정상적으로 인식되었습니다.\n\n')
	ktkws.stop()
	return rc, alarm

def main():
	test()

if __name__ == '__main__':
	main()
