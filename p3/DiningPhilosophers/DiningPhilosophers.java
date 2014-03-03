public class DiningPhilosophers
{

   public static void main(String args[])
   {
      DiningServer server = new DiningServerImpl();
      Philosopher[] philosopherArray = new Philosopher[DiningServerImpl.NUM_OF_PHILS];

     // create the philosopher threads
     for (int i = 0; i < DiningServerImpl.NUM_OF_PHILS; i++) philosopherArray[i] = new Philosopher(server,i);
     for (int i = 0; i < DiningServerImpl.NUM_OF_PHILS; i++) new Thread(philosopherArray[i]).start();


     while(true) {
       for (int i = 0; i < DiningServerImpl.NUM_OF_PHILS; i++) {
          if(server.getState(i) == DiningServer.State.THINK) System.out.print("." + " ");
          if(server.getState(i) == DiningServer.State.HUNGRY) System.out.print("h" + " ");
          if(server.getState(i) == DiningServer.State.EATING) System.out.print("E" + " ");
       }
       System.out.print("                  total:   ");
       for (int i = 0; i < DiningServerImpl.NUM_OF_PHILS; i++) {
          System.out.print(server.getTotalMeals(i) + " ");
       }
       System.out.println();

       Philosopher.nap(10);
     }  
   }
}
