#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
import ex1_kwstest as kws
import ex2_getVoice2Text as gv2t
import ex4_getText2VoiceStream as tts
import ex6_queryVoice as dss
import MicrophoneStream as MS
import time
import os
import vlc
import multiprocessing
import threading
import queue
import subprocess


VOLUME = 100

def find_time(result_mesg):
    res = []
    if '초' in result_mesg :
        location = result_mesg.index('초') - 1    # '초' 앞에 인덱스는 숫자 첫째자리 수
        res.append(result_mesg[location])
        while True:
            print(result_mesg[location])
            if (result_mesg[location-1] is None) or (result_mesg[location-1] not in ['0','1','2','3','4','5','6','7','8','9']) :
                # 그다음 앞에 인덱스가 숫자가 아니거나 아무것도 없을 때
                return res
            else:
                location -= 1
                res.append(result_mesg[location])
    return -1

def send_flag(flag):
    cnt = 0
    st = './send_' + str(flag) + '_flag'
    while True:
        cnt += 1
        data = subprocess.check_output([st])
        if 'fail' not in data.decode() : break
        if cnt > 10 : break

def main():
    #os.system('./send_start_flag')
    send_flag('start')

    KWSID = ['기가지니', '지니야', '친구야', '자기야']
    standard_time = -1
    second = 0
    que = queue.Queue()

    while True:
        recog, alarm = kws.btn_test(standard_time, second, que, KWSID[0])

        # 기가지니 호출 200
        if recog == 200:
            print("기가지니 호출됨")
            result_mesg = gv2t.getVoice2Text() #getVoice2Text_alarm()
            print(result_mesg)

            if ('알람' in result_mesg) and ( find_time(result_mesg) != -1):
                standard_time = time.time()
                print("알람을 찾았어요.")
                send_flag('start')

                second = find_time(result_mesg)
                second.reverse()
                second = int(''.join(second))
                print(second)

            elif ('코디' in result_mesg) :
                print("날씨에 따른 옷차림을 추천합니다.")
                send_flag('dress')
            
            elif ('외출' in result_mesg) :
                print("외출 전 마스크 및 우산 체크합니다.")
                send_flag('out')

                
        # 버튼 누르는 이벤트 100
        elif recog == 100:
            print('알람을 끕니다 스누즈 모드입니다.')
            alarm.stop()
            #alarm.stop()

            standard_time = time.time()
            second = 5
            alarm_off = vlc.MediaPlayer("alarm_off.mp3")
            alarm_off.audio_set_volume(VOLUME)
            alarm_off.play()

            send_flag('button')

        # 알람 울리는 이벤트 300
        elif recog == 300:
            print("알람 울리는 이벤트@@@@@@@@@@@@@2")
            standard_time = -1
            second = 0
            
if __name__ == '__main__':
    main()

