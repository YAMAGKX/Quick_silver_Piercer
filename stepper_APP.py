import streamlit as st
import serial
import time
import numpy as np


def force_prediction_func(x,a=2.70206061e-04,b=-4.63799561e-01,c=1.99811279e+02):
    return a*x**2+b*x+c

# Streamlit app
tab1, tab2= st.tabs(["Control", "reference_data"])
with tab1:
    st.title("Piercer  Controler")
    COM = st.number_input("Enter the number of COM:", min_value=1, value=3)

    # User input for commands
    step_count = st.number_input("Enter the number of steps:", min_value=1, value=1000)
    custom_delay = st.number_input("Enter the custom delay (microseconds):", min_value=1, value=4000)
    custom_limit = st.number_input("Enter the custom limit (push_sensor value):", min_value=0, value=500)

    if st.button('Push'):
        ser = serial.Serial(f"COM{COM}", 9600)
        ser.write(f"D {custom_delay}\n".encode())
        ser.write(f"L {custom_limit}\n".encode())
        ser.write(f"F {step_count}\n".encode())
        response = ser.readline().decode().strip()  # Read the response
        time.sleep(1)
        x=np.array(float(response)).reshape(-1,1)
        pred=force_prediction_func(x)
        st.write(f"min_sensor_val: {response}")
        st.write(f"Predicted_Force[N cm]: {'{:.2f}'.format(pred[0][0])}")
        ser.write(f"H {step_count}\n".encode())
        ser.close()



    if st.button('Homing'):
        ser = serial.Serial(f"COM{COM}", 9600)
        ser.write(f"D {custom_delay}\n".encode())
        ser.write(f"H {step_count}\n".encode())
        response = ser.readline().decode().strip()  # Read the response
        st.write(f"Response: {response}")
        ser.close()

with tab2:
    st.title("prediction reference data")
    st.write("20240719_ABJ")
    image_path = "PushSensor VS Force_Distance_3.4-0.9mm_fitting.png"
    st.image(image_path, use_column_width=True)


