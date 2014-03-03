import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class DiningServerImpl extends DiningServer
{  	

	private State[] state; // array to record each philosopher's state
	
	Lock lock;             // synchroization variables
	Condition[] self;
	
	public DiningServerImpl()
	{        
		state = new State[NUM_OF_PHILS];
		for (int i = 0; i < NUM_OF_PHILS; i++) state[i] = State.THINK;
		
		lock = new ReentrantLock();		
		self = new Condition[NUM_OF_PHILS];
		for (int i = 0; i < NUM_OF_PHILS; i++) self[i] = lock.newCondition();
	}

	// return the index of the left neighbor of philosopher i
	private int leftNeighbor(int i) {
		if (i == 0) return NUM_OF_PHILS - 1;
		else return i - 1;
    }
	
	// return the index of the right neighbor of philosopher i
	private int rightNeighbor(int i) {
		if (i == NUM_OF_PHILS - 1) return 0;
		else return i + 1;
	}

	public State getState(int philNumber) {
		return state[philNumber];
	}

	public int getTotalMeals(int philNumber) {
		return total[philNumber];
	}

    // **********************************************************************
    // Code to fill : takeForks and returnForks
	
	public void takeForks(int id) {
		// Exercice) Fill the code of takeForks,
		lock.lock();
		state[id] = State.HUNGRY;
		test(id);
		while(state[id] != State.EATING)
			try {
				self[id].await();
			} catch (InterruptedException e) {
				System.out.println(e);
			}
		lock.unlock();
    }
	
	public void returnForks(int id) {
		// Exercice) Fill the code of returnForks
		lock.lock();
		state[id] = State.THINK;
		//give left and right a chance to eat
		test(leftNeighbor(id));
		test(rightNeighbor(id));
		lock.unlock();
	}
	
	private void test(int id) {
		// if I'm hungry and my left and right nieghbors aren't eating, then let me eat!
		if(state[id] == State.HUNGRY && 
		   state[leftNeighbor(id)] != State.EATING &&
		   state[rightNeighbor(id)] != State.EATING) {
			state[id] = State.EATING;
			self[id].signal();
		}
	}






}
