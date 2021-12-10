//PLEASE RUN ON LINUX MACHINE , read README for further details

#include"server.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <limits.h>
//function to set all basic parameters to default settings
void default_settings ( struct atc_controller *ac )
{
    ac -> count = 0 ;
    ac -> front = 0 ;
    ac -> rear = -1 ;
}
//function to reduce the fuel in the plane after each unit since it keeps waiting in the landing queue 
void fuel_reduce(struct atc_controller *ac){
    for(int i = ac->front; i<ac->count; i++)
        ac->p[i].plane_fuel--;
}
//function to sort planes based on the fuel to allow least fuel plane to land first
void sort_planes_based_on_fuel(struct atc_controller *ac){
    for(int k = ac->front; k<ac->count; k++){
        for(int m = k+1; m<ac->count; m++){
            if( ac -> p[k].plane_fuel >= ac -> p[m].plane_fuel){
                struct plane_details t = ac -> p[k];
                ac -> p[k] = ac -> p[m];
                ac -> p[m] = t;
            }
        }
    }
}
//fucntion to add planes either to landing or takeoff queue
void add_planes( struct atc_controller *ac, struct plane_details item )
{
    if ( ac -> count >= max_no_of_planes )
    {
        printf ( "\nQueue is full_check.\n" ) ;
        return ;
    }
    ( ac -> count )++ ;
    ac -> rear = ( ac -> rear + 1 ) % max_no_of_planes ;
    ac -> p[ac -> rear] = item ;

    for(int k = ac->rear; k<ac->count; k++){
        for(int m = k+1; m<ac->count; m++){
            if( ac -> p[k].plane_fuel <= ac -> p[m].plane_fuel){
                struct plane_details t = ac -> p[k];
                ac -> p[k] = ac -> p[m];
                ac -> p[m] = t;
            }
        }
    }
}


//fucntion to check the number of planes through each unit of simulation
int count_check ( struct atc_controller q )
{
    return q.count ;
}
//fucntion to check if any queue is empty or not
int empty_check ( struct atc_controller q )
{
    return ( q.count <= 0 ) ;
}
//fucntion to check if any queue is full or not
int full_check ( struct atc_controller q )
{
    return ( q.count >= max_no_of_planes ) ;
}
//fucntion to set a starting point to the random number generator fucntion
void random_seed ( ) 
{
    srand ( ( unsigned int ) ( time ( NULL ) % 10000 ) ) ;
}
//fucntion to check if landing or takeoff queue is empty or not
int check_for_empty_queues ( struct airport_operations ap, char type )
{
    switch ( tolower ( type ) )
    {
        case'l' :
              return ( empty_check ( *( ap.pl ) ) ) ;

        case't' :
              return ( empty_check ( *( ap.pt ) ) ) ;
    }

    return 0 ;
}
//fucntion to check if landing or takeoff queue is filled or not
int check_for_filled_queues ( struct airport_operations ap, char type )
{
    switch ( tolower ( type ) )
    {
        case'l' :
              return ( full_check ( *( ap.pl ) ) ) ;

        case't' :
              return ( full_check ( *( ap.pt ) ) ) ;
    }

    return 0 ;
}
//fucntion to check the size of landing or takeoff queue
int landing_takeoff_queue_size ( struct airport_operations ap, char type )
{
    switch ( tolower ( type ) )
    {
        case'l' :
              return ( count_check ( *( ap.pl ) ) ) ;

        case't' :
              return ( count_check ( *( ap.pt ) ) ) ;
    }

    return 0 ;
}
//fucntion to add planes to landing or takeoff queue
void add_planes_to_queues ( struct airport_operations *ap, char type )
{
    switch ( tolower( type ) )
    {
        case'l' :
              add_planes ( ap -> pl, ap -> airplane ) ;
              break ;

        case't' :
              add_planes ( ap -> pt, ap -> airplane ) ;
              break ;
    }
}
//fucntion which generates random number based on POISSON DISTRIBUTION
int randomnumber ( double expectedvalue )
{
    int n = 0 ;
    double em ;
    double x ;

    em = exp ( -expectedvalue ) ;
    x = rand( ) / ( double ) INT_MAX ;

    while ( x > em )
    {
        n++ ;
        x *= rand( ) / ( double ) INT_MAX ;
    }

    return n ;
}
//fucntion to display all final statistics and end the simulation
void simulation_ending_control ( struct airport_operations *ap, int endtime )
{
    printf ( "\tSimulation has concluded after %d units.\n", endtime ) ;
    printf ( "\tTotal number of planes processed: %d\n", ap -> no_of_planes ) ;
    printf ( "\tNumber of planes landed: %d\n", ap -> no_of_planes_landed ) ;
    printf ( "\tNumber of planes taken off: %d\n", ap -> no_of_planes_takenoff ) ;
    printf ( "\tNumber of planes refused use: %d\n", ap -> no_of_planes_refused ) ;
    printf ( "\tNumber left ready to land: %d\n", landing_takeoff_queue_size ( *ap, 'l' ) ) ;
    printf ( "\tNumber left ready to take off: %d\n", landing_takeoff_queue_size ( *ap, 't' ) ) ;

    if ( endtime > 0 )
        printf ( "\tPercentage of time runway idle: %lf \n", ( ( double ) ap -> idletime / endtime ) * 100.0 ) ;

    if ( ap -> no_of_planes_landed > 0 )
        printf ( "\tAverage wait time to land: %lf \n", ( ( double ) ap -> landing_time_wait / ap -> no_of_planes_landed ) ) ;

    if ( ap -> no_of_planes_takenoff > 0 )
        printf ( "\tAverage wait time to take off: %lf \n", ( ( double ) ap -> takeoff_time_wait / ap -> no_of_planes_takenoff ) ) ;
}

//fucntion to accept input values from user and start the simulation
void simulation_begin_control ( int *endtime, double *expectarrive, double *expectdepart )
{
    int flag = 0 ;
    char wish ;

    printf ( "\nProgram that simulates an airport_operations with only one runway.\n" ) ;
    printf ( "One plane can land or depart in each unit of time.\n" ) ;
    printf ( "Up to %d planes can be waiting to land or take off at any time.\n", max_no_of_planes ) ;
    printf ( "How many units of time will the simulation run?" ) ;
    scanf ( "%d", endtime ) ;
    random_seed ( ) ;
    do
    {
        printf ( "\nExpected number of arrivals per unit time? " ) ;
        scanf ( "%lf", expectarrive ) ;
        printf ( "\nExpected number of departures per unit time? " ) ;
        scanf ( "%lf", expectdepart ) ;

        if ( *expectarrive < 0.0 || *expectdepart < 0.0 ) //ERROR HANDLING 
        {
            printf ( "These numbers must be nonnegative.\n" ) ;
            flag = 0 ;
        }
        else
        {
            if ( *expectarrive + *expectdepart > 1.0 ) //probability of landing+takeoff cannot be greaterthan 1
            {   //ERROR HANDLING 
                printf ( "The airport_operations will become saturated. Read new numbers? " ) ;
                scanf ( " %c", &wish ) ;
                if ( tolower ( wish ) == 'y' ) //ERROR HANDLING
                    flag = 0 ;
                else
                    flag = 1 ;
            }
            else
                flag = 1 ;
        }
    } while ( flag == 0 ) ;
}
//fucntion to register new plane details which will be added to either landing or takeoff queue
void new_plane_details ( struct airport_operations *ap, int curtime, int action )
{
    ( ap -> no_of_planes )++ ;
    ap -> airplane.plane_id = ap -> no_of_planes ;
    ap -> airplane.time_queue = curtime ;
    ap->airplane.plane_fuel = (rand() % (10 - 1 + 1)) + 1;

    switch ( action )
    {
        case arrive_choice :
            printf ( "\n" ) ;
            printf ( "Plane %d ready to land with fuel %d.\n", ap -> no_of_planes, ap->airplane.plane_fuel ) ;
            break ;

        case depart_choice :
            printf ( "\nPlane %d ready to take off.\n", ap -> no_of_planes ) ;
            break ;
    }
}
//fucntion to refuse a plane to land or takeoff when either landing or takeoff queues are full
void plane_refusing_authority ( struct airport_operations *ap, int action )
{
    switch ( action )
    {
        case arrive_choice :

             printf ( "\tplane  %d directed to another airport.\n", ap -> airplane.plane_id ) ;
             break ;

        case depart_choice :

             printf ( "\tplane %d told to try later.\n", ap -> airplane.plane_id ) ;
             break ;
    }
    ( ap -> no_of_planes_refused )++ ;
}
//fucntion to land the plane
void plane_landing_operation ( struct airport_operations *ap, struct plane_details pl, int curtime )
{
    int wait ;
    //sort_planes_based_on_fuel_based_on_fuel(ap->pl);
    wait = curtime - pl.time_queue ;
    printf ( "%d: Plane %d landed ", curtime, pl.plane_id ) ;
    printf ( "in queue %d units \n", wait ) ;
    ( ap -> no_of_planes_landed ) ++ ;
    ( ap -> landing_time_wait ) += wait ;
}
//function to takeoff a plane
void plane_takeoff_operation ( struct airport_operations *ap, struct plane_details pl, int curtime )
{
    int wait ;

    wait = curtime - pl.time_queue ;
    printf ( "%d: Plane %d took off ", curtime, pl.plane_id ) ;
    printf ( "in queue %d units \n", wait ) ;
    ( ap -> no_of_planes_takenoff )++ ;
    ( ap -> takeoff_time_wait ) += wait ;
}
//function to calculate the time in which runway was idle
void runway_idle_time_calculator ( struct airport_operations *ap, int curtime )
{
    printf ( "%d: Runway is idle.\n", curtime ) ;
    ap -> idletime++ ;
}
//function to set the landing and takeoff queues to basic settings
void default_airport_initialization ( struct airport_operations *ap )
{
    default_settings ( &( ap-> landing_queue ) ) ;
    default_settings ( &( ap -> takeoff_queue ) ) ;

    ap -> pl = &( ap -> landing_queue ) ;
    ap -> pt = &( ap -> takeoff_queue ) ;
    ap -> no_of_planes = ap -> no_of_planes_landed = ap -> no_of_planes_takenoff = ap -> no_of_planes_refused = 0 ;
    ap -> landing_time_wait = ap -> takeoff_time_wait = ap -> idletime = 0 ;
}



//fucntion which removes the plane from the FRONT of the queue
struct plane_details delete_planes ( struct atc_controller *ac )
{
    struct plane_details p1 ;

    if ( ac -> count <= 0 )
    {
        printf ( "\nQueue is empty_check.\n" ) ;
        p1.plane_id = 0 ;
        p1.time_queue = 0 ;
    }
    else
    {
        sort_planes_based_on_fuel(ac);
        ( ac -> count )-- ;
        p1 = ac -> p[ac -> front] ;
        ac -> front = ( ac -> front + 1 ) % max_no_of_planes ;
    }
    return p1 ;
}
//function which deletes the planes from either landing or takeoff queue
struct plane_details deletes_planes_from_queues ( struct airport_operations *ap, char type )
{
    struct plane_details p1 ;

    switch ( tolower ( type ) )
    {
        case'l' :
              p1 = delete_planes ( ap -> pl ) ;
              break ;

        case't' :
              p1 = delete_planes ( ap -> pt ) ;
              break ;
    }

    return p1 ;
}