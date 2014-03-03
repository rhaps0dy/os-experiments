public class Philosopher implements Runnable
{
	private DiningServer server;
	private int          philNum;
	

	public static void nap() { nap(5000); }
	
	public static void nap(int duration) {
		int sleeptime = 100 + (int) (duration * Math.random() );
		try { Thread.sleep(sleeptime); }
		catch (InterruptedException e) {}
	}

	public Philosopher(DiningServer server, int philNum)
	{
		this.server = server;
		this.philNum = philNum;
	}
	
	private void thinking()
	{
        nap(2000);
	}
	
	private void eating()
	{
		nap(2000);
    }
	
	// philosophers alternate between thinking and eating
	public void run()
	{
		while (true)
		{
			//System.out.println( philNum + " is thinking." );
			thinking();
			
			//System.out.println( philNum + " is hungry." );
			server.takeForks(philNum);
			
			//System.out.println( philNum + " is eating." );
			eating();
			
			//System.out.println( philNum + " is done eating." );
			server.returnForks(philNum);
		}
	}
}
