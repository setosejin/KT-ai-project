# %%
import cv2
import mediapipe as mp
mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_pose = mp.solutions.pose

# # For static images:
# IMAGE_FILES = []
# BG_COLOR = (192, 192, 192) # gray
# with mp_pose.Pose(
#     static_image_mode=True,
#     model_complexity=2,
#     enable_segmentation=True,
#     min_detection_confidence=0.5) as pose:
#   for idx, file in enumerate(IMAGE_FILES):
#     image = cv2.imread(file)
#     image_height, image_width, _ = image.shape
#     # Convert the BGR image to RGB before processing.
#     results = pose.process(cv2.cvtColor(image, cv2.COLOR_BGR2RGB))

#     if not results.pose_landmarks:
#       continue
#     print(
#         f'Nose coordinates: ('
#         f'{results.pose_landmarks.landmark[mp_pose.PoseLandmark.NOSE].x * image_width}, '
#         f'{results.pose_landmarks.landmark[mp_pose.PoseLandmark.NOSE].y * image_height})'
#     )

#     annotated_image = image.copy()
#     # Draw segmentation on the image.
#     # To improve segmentation around boundaries, consider applying a joint
#     # bilateral filter to "results.segmentation_mask" with "image".
#     condition = np.stack((results.segmentation_mask,) * 3, axis=-1) > 0.1
#     bg_image = np.zeros(image.shape, dtype=np.uint8)
#     bg_image[:] = BG_COLOR
#     annotated_image = np.where(condition, annotated_image, bg_image)
#     # Draw pose landmarks on the image.
#     mp_drawing.draw_landmarks(
#         annotated_image,
#         results.pose_landmarks,
#         mp_pose.POSE_CONNECTIONS,
#         landmark_drawing_spec=mp_drawing_styles.get_default_pose_landmarks_style())
#     cv2.imwrite('/tmp/annotated_image' + str(idx) + '.png', annotated_image)
#     # Plot pose world landmarks.
#     mp_drawing.plot_landmarks(
#         results.pose_world_landmarks, mp_pose.POSE_CONNECTIONS)

# For webcam input:
cap = cv2.VideoCapture(0)
with mp_pose.Pose(
    min_detection_confidence=0.5,
    min_tracking_confidence=0.5) as pose:
  while cap.isOpened():
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
    cv2.imshow('MediaPipe Pose', cv2.flip(image, 1))
    # 좌우반전없이
    
    
    
    # ESC 누르면 프로그램 종료
    # if cv2.waitKey(33) & 0xFF == 27:
    # 기지개 피면 프로그램 종료(기지개 : 양 손이 코보다 y좌표 30만큼 위에 있을 때)
    # if (cv2.waitKey(33) & (y_dist_r_wrist_to_nose >= 30 and y_dist_l_wrist_to_nose >= 30)):
    # ESC 누르거나 기지개 피면 프로그램 종료(기지개 : 양 손이 코보다 y좌표 30만큼 위에 있을 때)
    if (cv2.waitKey(33) & 0xFF == 27 or (y_dist_r_wrist_to_nose >= 30 and y_dist_l_wrist_to_nose >= 30)):
        # 기가지니에 알람 끄는 코드 넣기(실행중인 음악 파일 종료같은)

        # 2초동안 정지
        cv2.waitKey(2000)
        # 프로그램 종료
        break
cap.release()
cv2.destroyAllWindows()