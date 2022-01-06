# %%
import cv2
import os
import mediapipe as mp
import queue

mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_pose = mp.solutions.pose


def inference(que):
  # 자세를 감지하는 inference 시작
  cap = cv2.VideoCapture(0)
  y_dist_r_wrist_to_nose, y_dist_l_wrist_to_nose = 0, 0
  with mp_pose.Pose(
      min_detection_confidence=0.5,
      min_tracking_confidence=0.5) as pose:

    while cap.isOpened():
      # 반복 시작
      if not(que.empty()) :
        # Queue에 원소가 들어있으면 빼주고 inference 종료
        # 버튼이 눌리면 inference도 종료하기 위함
        que.get()
        break

      success, image = cap.read()
      if not success:
        print("Ignoring empty camera frame.")
        # If loading a video, use 'break' instead of 'continue'.
        continue

      # process 함수로 추론 시작
      image.flags.writeable = False
      image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
      results = pose.process(image)

      # 추론된 관절 위치 표시
      image.flags.writeable = True
      image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
      mp_drawing.draw_landmarks(
          image,
          results.pose_landmarks,
          mp_pose.POSE_CONNECTIONS,
          landmark_drawing_spec=mp_drawing_styles.get_default_pose_landmarks_style())
      
      
      # 0 : 코 / 16 : 오른손목 / 15 : 왼손목
      # 12 : 오른어깨 / 14 : 오른팔꿈치 / 16 : 오른손목
      # 11 : 왼어깨 / 13 : 왼팔꿈치 / 15 : 왼손목
      if results.pose_landmarks:
          nose = [int(results.pose_landmarks.landmark[0].x * 100), int(results.pose_landmarks.landmark[0].y * 100)]
          r_wrist = [int(results.pose_landmarks.landmark[16].x * 100), int(results.pose_landmarks.landmark[16].y * 100)]
          l_wrist = [int(results.pose_landmarks.landmark[15].x * 100), int(results.pose_landmarks.landmark[15].y * 100)]
          y_dist_r_wrist_to_nose = (nose[1] - r_wrist[1])
          y_dist_l_wrist_to_nose = (nose[1] - l_wrist[1])
      

      # 위치 지정하여 보여주기
      cv2.moveWindow('Pose', 630, 330)
      cv2.imshow('Pose', image) #cv2.flip(image, 1))

      if cv2.waitKey(1) == ord('q'):
          break

      # 왼쪽, 오른쪽 손목쪽 y값이 코의 y값보다 20 높다면!
      if (y_dist_r_wrist_to_nose >= 20) and (y_dist_l_wrist_to_nose >= 20):
        os.system('./send_check_flag')
        cv2.imwrite("/home/pi/Desktop/KT-ai-project/img/capture.jpg", image)
        cap.release()
        cv2.destroyAllWindows()
        return True
        break

    cap.release()
    cv2.destroyAllWindows()
    return True

  cap.release()
  cv2.destroyAllWindows()
