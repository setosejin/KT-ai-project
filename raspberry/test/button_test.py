from __future__ import print_function

import ex1_kwstest as kws

import ex4_getText2VoiceStream as tts

import ex6_queryVoice as dss

import MicrophoneStream as MS

import time

	

def main():

	#Example8 Button+STT+DSS

	KWSID = ['기가지니', '지니야', '친구야', '자기야']

	while 1:

		recog = kws.btn_test(KWSID[0])

		if recog == 200:

			print('200!')

			#time.sleep(2)

		else:

			print('KWS Not Dectected ...')



if __name__ == '__main__':

    main()


