#ifndef __CLOCK_H__
#define __CLOCK_H__

/**
 * Instances of these class provide a clock that sends ticks according to a predetermined period. Instances can behave
 * either as a simetrical clock or an asymetrical clock. In the first case, the instance should be created by means of
 * passing the constructor only one period of time simulation units. For the second case, instances of this class
 * should pass the constructor the 2 periods of time simulation units.
 */

#include <vector>
#include <simkernel.h>         // Hase++



class Clock : public sim_entity {
 private:
  std::vector<sim_entity_id> __entities;
  /** send to every entity the status information */
  void sendStatus();
  /** packet used to send the tick and status information together : 07.03.03*/
  int __packet[2];



 protected:


  /** @param value to be sent to each entity
   *  @return number of tick sent
   */
  int sendTick(int value);

  /** modify status parameter
   *  @param number of status to expect
   *  @param delay: wait status signals for delay at max.
   *  @return remaining delay
   */
  double waitStatus(double delay, int number);
  int __status;

  /** called when the simulation ends because of an entity requirement
   *  may use the status information to decide whether there was an error or not.
   *  Not called when the clock was too fast comparing to entities.
   */
  void end();

  /** @return Number of registered entities. */
  int getEntitiesNo() { return (int)__entities.size(); }

  public :
    static int CLOCK_TAG;
    int cycle;

  void registering(sim_entity_id ev_id);

  // cycle(out) should not be here
  Clock(const char *n, int cycle_i=0, double period_d=10);
  virtual void dumpState(int stage, char type);
  void dump_state(int stage=0);
  void dump_state_i(int stage=0);
  virtual void startup();
  virtual void body();

  double	period;			// Keeps the simulation time units for even clcok cycles
  double	period1;		// Ditto for odd clock cycles

   /**
	* Creates a new instance of the class.
	*
	* @param
	* @param cycle_i	The initial cycle
	* @param p0			The length in simulation time units of the even cycles
	* @param p1	 		ditto for odd cycles
	*/
	Clock(const char *n, int cycle_i, double period, double period1);

	/**
	 * This method returns the period of phase0
	 *
	 * @return	The period in simulaton time units
	 */
	double getPeriod();

	/**
	 * This method returns the period of phase1
	 *
	 * @return	The period in simulaton time units
	 */
	double getPeriod1();
};

#endif
