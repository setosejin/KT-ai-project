# %%
import cv2
import os
import mediapipe as mp
import queue

mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_pose = mp.solutions.pose


def inference(que):
  # For webcam input:
  cap = cv2.VideoCapture(0)
  y_dist_r_wrist_to_nose, y_dist_l_wrist_to_nose = 0, 0
  with mp_pose.Pose(
      min_detection_confidence=0.5,
      min_tracking_confidence=0.5) as pose:
    while cap.isOpened():
      if not(que.empty()) :
        que.get()
        break



      success, image = cap.read()
      if not success:
        print("Ignoring empty camera frame.")
        # If loading a video, use 'break' instead of 'continue'.
        continue

      # To improve performance, optionally mark the image as not writeable to
      # pass by reference.
      image.flags.writeable = False
      image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
      results = pose.process(image)

      # Draw the pose annotation on the image.
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
      
      # Flip the image horizontally for a selfie-view display.
      cv2.moveWindow('Pose', 630, 330)
      cv2.imshow('Pose', image)#cv2.flip(image, 1))

      if cv2.waitKey(1) == ord('q'):
          break

      if (y_dist_r_wrist_to_nose >= 20) and (y_dist_l_wrist_to_nose >= 20):
        print("detect guigeegae")
        os.system('./send_check_flag')
        cv2.imwrite("/home/pi/Desktop/KT-ai-project/img/capture.jpg", image)
        cap.release()
        cv2.destroyAllWindows()
        return True
        break

      # ESC 누르면 프로그램 종료
      # if cv2.waitKey(33) & 0xFF == 27:
      # 기지개 피면 프로그램 종료(기지개 : 양 손이 코보다 y좌표 30만큼 위에 있을 때)
      # if (cv2.waitKey(33) & (y_dist_r_wrist_to_nose >= 30 and y_dist_l_wrist_to_nose >= 30)):
      # ESC 누르거나 기지개 피면 프로그램 종료(기지개 : 양 손이 코보다 y좌표 30만큼 위에 있을 때)
      
      # if (cv2.waitKey(33) & 0xFF == 27 or (y_dist_r_wrist_to_nose >= 30 and y_dist_l_wrist_to_nose >= 30)):
      #     # 기가지니에 알람 끄는 코드 넣기(실행중인 음악 파일 종료같은)

      #     # 2초동안 정지
      #     cv2.waitKey(2000)
      #     # 프로그램 종료
      #     break
    cap.release()
    cv2.destroyAllWindows()
    return True

  cap.release()
  cv2.destroyAllWindows()
