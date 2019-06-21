# -*- coding: utf-8 -*-
"""
Created on Sat May 18 10:51:07 2019

@author: dhsimpson
"""

import cv2
import time
import numpy as np

# ���� ������ �ǴܵǴ� ��ǥ���� x��ǥ ����� ��ȯ�ϴ� ��
def get_Center_Of_Red(cap):
    center_arr = 0
    center = 0
    count = 0
    ret, frame=cap.read()
    
    if ret:
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        lower_red = np.array([0,50,50])
        upper_red = np.array([10,255,255])
        
        mask = cv2.inRange(hsv, lower_red, upper_red)
        res = cv2.bitwise_and(frame,frame, mask = mask)
        
        lower_red2 = np.array([170,50,50])
        upper_red2 = np.array([180,255,255])
        
        mask2 = cv2.inRange(hsv, lower_red2, upper_red2)
        res2 = cv2.bitwise_and(frame,frame, mask = mask2)
        
        red_Masked = cv2.addWeighted(res,0.5,res2,0.5,0)
        
        bit_Red_Masked = cv2.cvtColor(red_Masked, cv2.COLOR_BGR2GRAY)
        
        check = 0
        for i in range(0,bit_Red_Masked.shape[0]):
            check = 0
            Start = 0
            End = 0
            for j in range(0, bit_Red_Masked.shape[1]):
                if bit_Red_Masked[i,j] != 0:
                    if check == 0:
                        Start = j
                    check += 1
            End = Start + check - 1
            
            if check != 0:
                center_arr += (Start + End)/2
                count += 1
        center = center_arr / count
        return center

def get_360_Degree_Time():
    
    start_Red = 0
    end_Red = 0
    gap_Red =0
    
    #��� ȭ�� ���� �� - ���� ���� �߽� ��ǥ �ľ�
    cap = cv2.VideoCapture(0)
    ret, frame=cap.read()
    start_Red = get_Center_Of_Red(cap)
    
    past_Time = time.time()
    current_Time = 0.0
    passed_Time = 0.0
    
    
    ret, frame=cap.read()
    #��� ȸ�� ���� - ���� ���� �߽� ��ǥ �ǽð� �ľ��Ͽ� ���� ���� �߽� ��ǥ�� ��
    #                ���� ���� ���� �߽� ��ǥ�� ���� ���� �߽� ��ǥ�� ������(ȭ�Ҽ� 10%) �̳��� 
    #                ������ �� ���� ȭ���� ������ �Ǵ� 
    while ret:
        end_Red = get_Center_Of_Red(cap)
        gap_Red = end_Red - start_Red
        
        if gap_Red<0:
            gap_Red *= -1
            
        current_Time = time.time()
            
        passed_Time = current_Time - past_Time 
        print(passed_Time,":",gap_Red)
        
        
        if passed_Time > 10:
            print("�ٽ� �õ��� ������.")
            return 0.0
        else:
            if gap_Red<=10 and passed_Time > 2:
                print("�� ���� ȸ���߽��ϴ�.")
                break
        ret, frame=cap.read()
    return passed_Time
        
    
