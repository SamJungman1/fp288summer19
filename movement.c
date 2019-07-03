#include <movement.h>
#include <open_interface.h>

void move_forward(oi_t *sensor_data, int millimeters)
{
    int sum = 0;
    oi_setWheels(250, 250); // move forward; half speed
    while (sum < millimeters)
    {
        oi_update(sensor_data);
        sum += sensor_data->distance;
    }

    oi_setWheels(0, 0); //stop
}

void move_backward(oi_t *sensor_data, int millimeters)
{
    int sum = 0;
    oi_setWheels(-250, -250); // move backward; half speed
    while (sum > -millimeters)
    {
        oi_update(sensor_data);
        sum += sensor_data->distance;
    }
    oi_setWheels(0, 0); //stop

}

void turn_clockwise(oi_t *sensor_data, double degrees)
{
    double deg = 0;
    oi_setWheels(-100, 100); //turn clockwise
    while (deg > degrees)
    {
        oi_update(sensor_data);
        deg += sensor_data->angle;
    }
    oi_setWheels(0, 0); //stop
}

void turn_counterclockwise(oi_t *sensor_data, double degrees)
{
    double deg = 0;
    oi_setWheels(100, -100); //turn clockwise
    while (deg < degrees)
    {
        oi_update(sensor_data);
        deg += sensor_data->angle;
    }
    oi_setWheels(0, 0); //stop
}

int move_forward_collision_detect(oi_t *sensor_data, int millimeters)
{
    int sum = 0;
    oi_setWheels(250, 250); // move forward; half speed
    while (sum < millimeters)
    {
        oi_update(sensor_data);
        sum += sensor_data->distance;

        if (sensor_data->bumpLeft == 1)
        {
            move_backward(sensor_data, 150);
            sum = sum - 150;
            turn_clockwise(sensor_data, -100); //                              //
            move_forward(sensor_data, 250);         //                        //
            turn_counterclockwise(sensor_data, 100);

        }
        if (sensor_data->bumpRight == 1)
        {
            move_backward(sensor_data, 150);
            sum = sum - 150;
            turn_counterclockwise(sensor_data, 100); //                              //
            move_forward(sensor_data, 250);         //                        //
            turn_clockwise(sensor_data, -100);
        }
        oi_setWheels(250,250);

    }

    oi_setWheels(0, 0); //stop

    return sum;
}
