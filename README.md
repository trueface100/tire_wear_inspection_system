# tire_wear_inspection_system
### 에지 검출과 이미지 분류를 통한 타이어 마모 점검 시스템
2023년 2학기 AI 임베디드 프로그래밍 기말 프로젝트 결과물입니다. 이미지 분류를 통해 타이어의 마모 상태를 점검하는 시스템의 프로토타입을 제작하였습니다.

## 👨‍🏫프로젝트 소개
"에지 검출과 이미지 분류를 통한 타이어 마모 점검 시스템"이라는 주제의 프로젝트입니다.
LeNet 모델을 변형하여 타이어 이미지를 학습하도록 하였고 온전한 타이어와 마모된 타이어를 분류합니다.
수업 중 공부하고 실습한 라즈베리파이, 센서, 엑츄에이터를 활용하여 프로토타입을 테스트할 모형 차량 출입구를 제작하였습니다.

## ⏲개발 기간
2023년 9월 ~ 2023년 11월

## 🧑‍🤝‍🧑개발자
팀원A: 최민석(본인)
팀원B: 강세륜(팀원)

## 📜프로젝트 소스코드 목록
* main_code: 라즈베리파이, 센서, 엑츄에이터의 동작 관련 코드
* performanceTool: 모델 성능평가 도구
* takePictureTool: 학습에 사용될 사진촬영 및 에지 검출 자동화 도구
* canny_good: 모델 학습 입력 데이터(온전한 타이어)
* canny_bad: 모델 학습 입력 데이터(마모된 타이어)
* test_data: 성능 평가 입력 데이터
* EAI_tireD_lenet.ipynb: 모델 생성 및 학습, TFLite로 변환 관련 코드

## 🧱프로토타입 결과물
![image](https://github.com/user-attachments/assets/db45eba0-9e75-4532-9776-24f2ffb7769d)<br>
<모형 차량 출입구 구조>

[![Video Label](http://img.youtube.com/vi/vS1r80bjqQ4/0.jpg)](https://youtu.be/vS1r80bjqQ4)<br>
![image](https://github.com/user-attachments/assets/2200ee7e-2ecf-4d44-a2b3-29f153e2382d)<br>
<프로토타입 시연 영상(마모된 타이어)>

[![Video Label](http://img.youtube.com/vi/Uj8V2BtA_cc/0.jpg)](https://youtu.be/Uj8V2BtA_cc)<br>
![image](https://github.com/user-attachments/assets/f81ecad1-e7de-4382-b171-370e22436645)<br>
<프로토타입 시연 영상(온전한 타이어)>

![image](https://github.com/user-attachments/assets/7cb84724-306d-4d99-a081-40b67f60d6d6)<br>
![image](https://github.com/user-attachments/assets/070e8d6f-bf35-4abb-b160-308f16362d30)<br>
평균 추론 시간: 약 0.2 sec

![image](https://github.com/user-attachments/assets/aedeb8f6-820b-4d8e-8171-761b28e2d168)<br>
precision: 0.85
recall: 0.87
accuracy: 0.86
F1: 0.86


