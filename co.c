#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include"server.h"

//PLEASE RUN ON LINUX MACHINE , read README for further details
int main( )
{   
    struct airport_operations a ;
    int i, randno, current_time, ending_time ;
    double average_arrival_per_unit, average_departure_per_unit ;
    struct plane_details ils ;

    default_airport_initialization ( &a );

    simulation_begin_control ( &ending_time, &average_arrival_per_unit, &average_departure_per_unit ) ;

    for ( current_time = 1 ; current_time <= ending_time ; current_time++ )
    {
        printf("\n\n");
        randno = randomnumber( average_arrival_per_unit ) ; //randno takes the value of a pseudo random integer based on the POISSON DISTRIBUTION for generating random numbers
        //adding planes to landing queue
        for ( i = 1 ; i <= randno ; i++ )
        {   
            sort_planes_based_on_fuel((&a)->pl);
            new_plane_details ( &a, current_time, arrive_choice ) ;
            if ( check_for_filled_queues ( a, 'l' ) )
                 plane_refusing_authority ( &a, arrive_choice ) ;
            else{
                sort_planes_based_on_fuel((&a)->pl);
                add_planes_to_queues( &a, 'l' ) ;
                sort_planes_based_on_fuel((&a)->pl);
                }
        }
        //adding planes to takeoff queue
        randno = randomnumber( average_departure_per_unit ) ; //randno takes the value of a pseudo random integer based on POISSION DISTRIBUTION for generating random numbers 
        for ( i = 1 ; i <= randno ; i++ )
        {
            new_plane_details ( &a, current_time, depart_choice ) ;
            if ( check_for_filled_queues ( a, 't' ) )
               plane_refusing_authority ( &a, depart_choice ) ;
            else{
                add_planes_to_queues ( &a, 't' ) ;
                sort_planes_based_on_fuel((&a)->pl);
            }
        }
        //if landing queue is not empty it lands a plane therefore deleting the plane from the landing queue
        if (  ! ( check_for_empty_queues ( a, 'l' ) ) )
        {
            ils = deletes_planes_from_queues ( &a, 'l' ) ;
            sort_planes_based_on_fuel((&a)->pl);
            plane_landing_operation ( &a, ils, current_time ) ;
            sort_planes_based_on_fuel((&a)->pl);
        }
        else
        {   //if takeoff queue is not empty it takeoff's a plane thereby deleting the plane from the takeoff queue
            if ( ! ( check_for_empty_queues ( a, 't' ) ) )
            {
                ils = deletes_planes_from_queues ( &a, 't' ) ;
                plane_takeoff_operation ( &a, ils, current_time ) ;
                sort_planes_based_on_fuel((&a)->pl);
            }
            else{ //if both the landing and takeoff queue is empty then runway is idle
                runway_idle_time_calculator ( &a, current_time ) ;
                sort_planes_based_on_fuel((&a)->pl);
            }
                
        }

        fuel_reduce((&a)->pl); //function to reduce the fuel in the plane after each unit since it keeps waiting in the landing queue 
        sort_planes_based_on_fuel((&a)->pl);//sort the planes based on fuel in landing queue to allow the least fuel plane to land first
    }

    simulation_ending_control ( &a, ending_time ) ; //simulation ends after units specified by the user
    return 0;
}