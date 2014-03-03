abstract public class DiningServer 
{
   static int NUM_OF_PHILS = 5;

   int[] total;  // Total meals taken by a phiolosopher to control starvation

   static enum State {THINK, HUNGRY, EATING};    // different philosopher states

   public DiningServer() {
		total = new int [NUM_OF_PHILS]; 
		for (int i = 0; i < NUM_OF_PHILS; i++) total[i] = 0;
   } 

   abstract public void takeForks(int philNumber);    // called by a philosopher when they wish to eat  
   abstract public void returnForks(int philNumber);  // called by a philosopher when they are finished eating 
 
   abstract public State getState(int philNumber);    // To get the state of a philosopher for example to print it
   abstract public int getTotalMeals(int philNumber); // To access the total number of meals for printing
}
