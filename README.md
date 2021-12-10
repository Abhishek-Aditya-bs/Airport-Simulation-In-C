# Airport-Simulation-In-C

Simulation of the working of a busy airport.

## Problem Requirements

Let us consider a small but busy airport with only one runway. In each unit of time (minute, hour), one plane can land or one plane can take off, but not both. Planes arrive ready to land or ready to take off at random times, so at any given unit of time, the runway may be idle or a plane may be landing or taking off and there may be several planes waiting either to land or take off. We therefore need two queues, called landing and takeoff, to hold these planes. It is better to keep a plane waiting on the ground than in the air, so a small airport allows a plane to take off only if there are no planes waiting to land. When each plane arrives to land, it will (as part of its data) have a (randomly generated) fuel level, measured in units of time remaining. If the plane does not have enough fuel to wait in the queue, it is allowed to land immediately. Hence the planes in the landing queue may be kept waiting additional units and so may run out of fuel themselves. Handling this as a part of the landing function and to find about how busy the airport can become before planes start to crash from running out of fuel.

## Explanation about the program

The program takes the AVERAGE ARRIVAL PER UNIT TIME and AVERAGE DEPARTURE PER UNIT TIME inputs from the user and also for how many units should the simulation run input from the user and based on the POISSON DISTRIBUTION of generating pseudo random numbers it generates random fuel for each plane and random number for adding planes to landing or takeoff queue.landing a plane means removing a plane from the FRONT of the queue and deleting it from the landing queue ,same happens in taking off a plane . Adding a plane means increasing the REAR index of the queue by one and adding to the rear position of the queue either landing or takeoff queue.If both landing and takeoff queue is empty means there is no plane to land or takeoff so runway is idle. simulation runs through the units specified by the user and displays the final statistics like number of planes landed, taken off, refused to land or takeoff etc, and thus the simulation ends.

## Assumptions

1. random fuel for each plane is assumed to be in the range 1-10
2. maximum number of planes in either landing or takeoff queue is 3
3. the randomnumber ( double ) function generates a random number based on
poisson distribution curve . Let’s say the average of n numbers is 4.6 ,the list of n numbers whose average may be 3.8 will be from a set S={4,7,1,...} and this function picks a random number from that list. Lets suppose 2 planes arrive in every 5 units of time ,then the expected number of arrivals is 0.4 since our average is
0.4 ,we use this function to pick out random numbers that may eventually average out to 0.4

## Working Criteria

Unfortunately ,this randomnumber(double) function only works on a LINUX machine, because rand( )/(double) INT_MAX always returns zero for windows , making the runways idle at all times. Please execute this program on a LINUX machine.
