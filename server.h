//PLEASE RUN ON LINUX MACHINE , read README for further details

#define max_no_of_planes 3 //max number of plane at a time which can either land or take off
#define arrive_choice 0    
#define depart_choice 1

//structure to store plane details
struct plane_details
{
    int plane_id ;    //plane id which equals to the count of the plane in the landing or take off queue
    int time_queue ;    //plane arrival time in the queue
    int plane_fuel;   //plane fuel units 
} ;
 
//structure which controls all landing and takeoff operations
struct atc_controller
{
    int count ;  //stores the count of the number of planes through each unit of simulation
    int front ;  //front stores the index of the first plane added to the landing or take off queue
    int rear ;   //rear stores the index of the last plane added to either landing or take off queue
    struct plane_details p[max_no_of_planes] ; //array of plane structures which stores individual plane_details information
} ;
//structure used to store additional details
struct airport_operations
{
    struct atc_controller landing_queue ;
    struct atc_controller takeoff_queue ;
    struct atc_controller *pl ; //queue which stores landing plane details 
    struct atc_controller *pt ; //queue which stores the take off plane details
    int idletime ; //stores the time in which runway is idle 
    int landing_time_wait, takeoff_time_wait ; //stores the time of delay of landing or takeoff
    int no_of_planes_landed, no_of_planes, no_of_planes_refused, no_of_planes_takenoff ;
    struct plane_details airplane ;
} ;

//various fucntions declarations

void default_airport_initialization ( struct airport_operations * ) ;
void simulation_begin_control ( int *, double *, double * ) ;
void new_plane_details ( struct airport_operations *, int, int ) ;
void plane_refusing_authority ( struct airport_operations *, int ) ;
void plane_landing_operation ( struct airport_operations *, struct plane_details, int ) ;
void plane_takeoff_operation ( struct airport_operations *, struct plane_details, int ) ;
void runway_idle_time_calculator ( struct airport_operations *, int ) ;
void simulation_ending_control ( struct airport_operations *, int ) ;
int randomnumber ( double ) ;
void add_planes_to_queues ( struct airport_operations *, char ) ;
struct plane_details deletes_planes_from_queues ( struct airport_operations *, char ) ;
int landing_takeoff_queue_size ( struct airport_operations, char ) ;
int check_for_filled_queues ( struct airport_operations, char ) ;
int check_for_empty_queues ( struct airport_operations, char ) ;
void random_seed ( ) ;
void default_settings ( struct atc_controller * ) ;
void add_planes ( struct atc_controller *, struct plane_details ) ;
struct plane_details delete_planes ( struct atc_controller * ) ;
int count_check ( struct atc_controller ) ;
int empty_check ( struct atc_controller ) ;
int full_check ( struct atc_controller ) ;
void sort_planes_based_on_fuel(struct atc_controller *ac);
void fuel_reduce(struct atc_controller *ac);
