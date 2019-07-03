/*
 * lab_9_template.c
 *
 *  Created on: Jun 26, 2019
 *      Author: colemata
 */

#include "timer.h"
#include "ping.h"
#include "servo.h"
#include "uart.h"
#include "adc.h"
#include "lcd.h"
#include "math.h"
#include "tm4c123gh6pm.h"

volatile int time_diff = 0;
enum state
{
    FOUND, NOT_FOUND
};
enum state detect = NOT_FOUND;

int main(void)
{
    lcd_init();
    uart_init();
    adc_init();
    gpio_portb_init();
    gpio_porte_init();
    servo_init();
    ping_init();
    timer_waitMillis(1000);

    struct OBJ
    {
        double start_degree;
        double end_degree;
        int start_distance;
        int end_distance;
        double diff_degree;
        double obj_size;
        double obj_center;

    };

    struct OBJ detected[15];

    char title[50] = "Degrees\t\tIR Distance (cm)\tSonar Distance (cm)\r\n";    //title
    char data[100] = 0;         //removing these zeroes breaks the putty display. DONT REMOVE THE ZEROS!!!!!
    int ir_value[90] = 0;       //stores IR scan values
    int sonar_value[90] = 0;    //stores sonar scan values
    int degree_value[90] = 0;   //stores degree values

    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int m = 0;

    double degrees = 0;         //degree of servo

    int ir_distance = 0;        //distance of IR sensor

    double clock_time = 0;
    int sonar_distance = 0;

    char ascii = 0;

    int obj_amt = 0;

    int smallest_object = 0;
    int object_number = 0;

    while (1)
    {
        ascii = uart_receive();     //obtains keyboard press from putty
        if (ascii == 'm')
        {
            servo_reset();          //sets servo to 0 degrees
            timer_waitMillis(1000);
            for (i = 0; i < 50; i++)    //for loop to send title line to putty
            {
                uart_sendChar(title[i]);
            }
            degrees = 0;            //resets degrees
            sprintf(data, "%lf\t%d\t\t\t%d\r\n", degrees, ir_distance, sonar_distance); //puts all relevant data into the data array
            for (i = 0; i < 100; i++)   //for loop to send data array to putty
            {
                uart_sendChar(data[i]);
            }
            l = 0; k = 0; j = 0;
            while (degrees < 180)
            {
                degrees = servo_move(degrees);  //sweeps 2 degrees at a time
                degree_value[l] = degrees;      //stores degrees into the array
                l++;                            //increments degree array

                timer_waitMillis(100);
                ping_read(&time_diff);                              //this line and the next 2 are for computing the sonar distance
                clock_time = (double) time_diff / 16000000;
                sonar_distance = clock_time * 340 * 100 / 2;

                sonar_value[k] = sonar_distance;    //stores sonar value into the array
                k++;                                //increments sonar array

                ir_distance = adc_do(ir_distance);  //computes IR distance

                ir_value[j] = ir_distance;          //stores IR value into the array
                j++;                                //increments IR array

                sprintf(data, "%lf\t%d\t\t\t%d\r\n", degrees, ir_distance, sonar_distance); //puts all relevant data into the data array

                for (i = 0; i < 100; i++)
                {
                    uart_sendChar(data[i]);
                }
            }
            obj_amt = 0;
            for(m = 0; m < 15; m++) {                       //this for loop resets the entire struct to 0;
                detected[m].start_degree = 0;
                detected[m].end_degree = 0;
                detected[m].start_distance = 0;
                detected[m].end_distance = 0;
                detected[m].diff_degree = 0;
                detected[m].obj_size = 0;
                detected[m].obj_center = 0;

            }
            detect = NOT_FOUND;
            for (i = 1; i < 90; i++)
            {
                if (detect == NOT_FOUND && ir_value[i] < 20 && sonar_value[i] < 20)     //object is found at less than 20 cm
                {
                    detect = FOUND;
                    detected[obj_amt].start_degree = degree_value[i];                   //stores degree object is first found
                    detected[obj_amt].start_distance = ir_value[i];                     //stores distance from sensor to object first found

                }
                else if (detect == FOUND && ir_value[i] > 20 && sonar_value[i] > 10)    //object is no longer found for IR > 20 cm and sonar < 10 cm
                {
                    detect = NOT_FOUND;
                    detected[obj_amt].end_degree = degree_value[i];                     //stores degree object is last seen
                    detected[obj_amt].end_distance = ir_value[i];                       //stores distance from sensor to object last seen
                    detected[obj_amt].diff_degree = detected[obj_amt].end_degree - detected[obj_amt].start_degree;  //stores degree for the cosine calculation
                    detected[obj_amt].obj_center = (detected[obj_amt].end_degree + detected[obj_amt].start_degree) / 2; //finds the object's center point
                    detected[obj_amt].obj_size = sqrt(pow(detected[obj_amt].start_distance, 2) + pow(detected[obj_amt].end_distance, 2) - 2 * detected[obj_amt].start_distance * detected[obj_amt].end_distance * cos(detected[obj_amt].diff_degree * 3.14 / 180)); //law of cosines
                    obj_amt++;
                }
            }
            smallest_object = 500;
            for (i = 0; i < obj_amt; i++)                           //sets smallest size to 500 and goes through all detected objects to find the smallest one
            {
                if (detected[i].obj_size < smallest_object)
                {
                    smallest_object = detected[i].obj_size;
                    object_number = i;
                }
            }
        lcd_printf("Objects detected: %d\nSmallest object: #%d\nObject location: %d\n", obj_amt, object_number, (int)detected[object_number].obj_center);
        servo_point(detected[object_number].obj_center);

        }

    }
}
