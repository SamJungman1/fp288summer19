#ifndef MOVEMENT_H_
#define MOVEMENT_H_
#include <open_interface.h>

//moves bot forward by x millimeters
void move_forward(oi_t *sensor_data, int millimeters);

//moves bot backward by x millimeters
void move_backward(oi_t *sensor_data, int millimeters);

//turns bot clockwise by x degrees
void turn_clockwise(oi_t *sensor_data, double degrees);

//turns bot counterclockwise by x degrees
void turn_counterclockwise(oi_t *sensor_data, double degrees);

int move_forward_collision_detect(oi_t *sensor_data, int millimeters);

#endif /* MOVEMENT_H_ */
