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

# 볼륨
VOLUME = 70


def find_time(result_mesg):
    # 인식된 문장에서 시간을 찾는 함수
    res = []
    if '초' in result_mesg :
        location = result_mesg.index('초') - 1    # '초' 앞에 인덱스는 숫자 첫째자리 수
        res.append(result_mesg[location])   
        while True:
            if (result_mesg[location-1] is None) or (result_mesg[location-1] not in ['0','1','2','3','4','5','6','7','8','9']) :
                # 그다음 앞에 인덱스가 숫자가 아니거나 아무것도 없을 때
                return res
            else:
                # 그다음 앞 인덱스도 찾기 위해 location -1,  res 배열에 넣기
                location -= 1
                res.append(result_mesg[location])
    return -1

def send_flag(flag):
    # IoT Makers에 Open API를 실패하면 재전송하는 함수 
    cnt = 0
    st = './send_' + str(flag) + '_flag'
    while True:
        cnt += 1
        data = subprocess.check_output([st])
        if 'fail' not in data.decode() : break
        if cnt > 10 : break     # 10회 이상 실패시 종료

def main():
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
            result_mesg = gv2t.getVoice2Text()
            print(result_mesg)

            if ('알람' in result_mesg) and ( find_time(result_mesg) != -1) and ('초' in result_mesg) :
                # 인식된 문장에 초랑 알람이라는 단어가 들어가는지, 몇 초인지 인식이 안되는지
                standard_time = time.time()
                set_alarm = vlc.MediaPlayer("set_alarm.mp3")
                set_alarm.play()
                print("알람을 찾았어요.")
                send_flag('start')

                second = find_time(result_mesg)
                second.reverse()
                second = int(''.join(second))
                print(second)

            elif ('코디' in result_mesg) :
                # 인식된 문장에 코디라는 단어가 들어가는지
                cloth = vlc.MediaPlayer("cloth.mp3")
                cloth.play()
                print("날씨에 따른 옷차림을 추천합니다.")
                send_flag('dress')
            
            elif ('외출' in result_mesg) :
                # 인식된 문장에 외출이라는 단어가 들어가는지
                out = vlc.MediaPlayer("out.mp3")
                out.play()
                print("외출 전 마스크 및 우산 체크합니다.")
                send_flag('out')

        # 버튼 누르는 이벤트 100
        elif recog == 100:
            print('알람을 끕니다 스누즈 모드입니다.')
            alarm.stop()
            que.put(1) # queue에 1을 넣어 버튼이 눌렸다는 것을 알린다.

            standard_time = time.time()
            second = 5
            alarm_off = vlc.MediaPlayer("alarm_off.mp3")
            alarm_off.audio_set_volume(VOLUME)
            alarm_off.play()

            send_flag('button')

        # 알람 울리는 이벤트 300
        elif recog == 300:
            print("알람이 울립니다.")
            standard_time = -1
            second = 0
            

if __name__ == '__main__':
    main()

