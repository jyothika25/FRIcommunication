package com.kuka.connectivity.fri.example;

import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import javax.inject.Inject;
import com.kuka.connectivity.fastRobotInterface.ClientCommandMode;
import com.kuka.connectivity.fastRobotInterface.FRIChannelInformation;
import com.kuka.connectivity.fastRobotInterface.FRIChannelInformation.FRIConnectionQuality;
import com.kuka.connectivity.fastRobotInterface.FRIConfiguration;
import com.kuka.connectivity.fastRobotInterface.FRIJointOverlay;
import com.kuka.connectivity.fastRobotInterface.FRISession;
import com.kuka.connectivity.fastRobotInterface.IFRISessionListener;
import com.kuka.roboticsAPI.applicationModel.RoboticsAPIApplication;
import static com.kuka.roboticsAPI.motionModel.BasicMotions.*;

import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.motionModel.PTP;
import com.kuka.roboticsAPI.motionModel.RobotMotion;

/**
 * Implementation of a robot application.
 * <p>
 * The application provides a {@link RoboticsAPITask#initialize()} and a 
 * {@link RoboticsAPITask#run()} method, which will be called successively in 
 * the application life cycle. The application will terminate automatically after 
 * the {@link RoboticsAPITask#run()} method has finished or after stopping the 
 * task. The {@link RoboticsAPITask#dispose()} method will be called, even if an 
 * exception is thrown during initialization or run. 
 * <p>
 * <b>It is imperative to call <code>super.dispose()</code> when overriding the 
 * {@link RoboticsAPITask#dispose()} method.</b> 
 * 
 * @see UseRoboticsAPIContext
 * @see #initialize()
 * @see #run()
 * @see #dispose()
 */
public class TestFRI extends RoboticsAPIApplication {
	
	
	private String clientName;
	@Inject
	private LBR lbr;
	
	@Override
	public void initialize() 
	{
		// initialize your application here
		clientName = "172.30.0.122";
		
		
		
	}

	@Override
	public void run() 
	{
		// your application execution starts here
		 FRIConfiguration friConfiguration = FRIConfiguration.createRemoteConfiguration(lbr, clientName);
	     friConfiguration.setSendPeriodMilliSec(5);
	     friConfiguration.setReceiveMultiplier(1);
	     
	     FRISession friSession = new FRISession(friConfiguration);
	     getLogger().info("Creating FRI connection to " + friConfiguration.getHostName());
	     getLogger().info("SendPeriod: " + friConfiguration.getSendPeriodMilliSec() + "ms |"
	                            + " ReceiveMultiplier: " + friConfiguration.getReceiveMultiplier());
	     
	     
	     FRIJointOverlay ov = new FRIJointOverlay(friSession,ClientCommandMode.POSITION);
	     
	    
	        // wait until FRI session is ready to switch to command mode 
		 try
	        {
	            friSession.await(100, TimeUnit.SECONDS);

	            
	        }   
	        catch (final TimeoutException e)
	        {
	        	FRIChannelInformation channInfo = friSession.getFRIChannelInformation();
	            getLogger().error("Timeout occured - quality: "+ channInfo.getQuality() + " - session state: "+ channInfo.getFRISessionState());
	        
	            
	            friSession.close();
	            return;
	        }
		    
		     
		     getLogger().info("FRI Connection to Client established");
		     
		     
		     //Monitoring Jitter, latency and quality of the FRI connection, Time stamp at which the quality change occurred & Current FRI state

		     IFRISessionListener listener = new IFRISessionListener(){
		    	 @Override
		    	 public void onFRIConnectionQualityChanged(FRIChannelInformation channelInfo)
		    	 {
		    	 
		    	  getLogger().info(" SessionqualityChangedto: "+ channelInfo.getQuality() );
		    	  }
		    	 @Override
		    	 public void onFRISessionStateChanged(FRIChannelInformation channelInfo)
		    	 {
		    		 getLogger().info(" SessionStateChangedto:" + channelInfo.getFRISessionState()+ " | Jitter:"+channelInfo.getJitter() + "ms |"
		     		           +" | Latency:"+channelInfo.getLatency() + "ms |" + " | TimeStamp:"+channelInfo.getTimeStampMillis() + "ms |");
		    	  }
		    	 };
		    	 friSession.addFRISessionListener(listener); 

		     
		     
		     
		    lbr.move(ptp(0.0,0.0,0.0,0.0,0.0,0.0,0.0));
		    getLogger().info("Switching to Command mode, motion starts");
		    lbr.move(ptp(Math.toRadians(20), .0, .0, Math.toRadians(20), .0, Math.toRadians(-20), .0).setJointVelocityRel(0.1).addMotionOverlay(ov));
		    getLogger().info("CurrentJointPosition= " + lbr.getCurrentJointPosition());
		    getLogger().info("CommandedJointPosition" + lbr.getCommandedJointPosition());
            lbr.move(ptp(0.0,0.0,0.0,0.0,0.0,0.0,0.0));
            lbr.move(ptp( 0.5, 0.8, 0.2, 1.0, -0.5, -0.5, -0.7) .setJointVelocityRel(0.2));
            
            lbr.move(ptp(-0.5, -0.8, -0.2, -1.0, 0.5, 0.5, 0.7).setJointVelocityRel(0.2));
            lbr.move(ptp(0.0,0.0,0.0,0.0,0.0,0.0,0.0));
            
            
      
             
            
            
            
            
            // done
		    getLogger().info("Closing Session");
		    friSession.close();
         
		    /*boolean t = true;
		while (t)
	 	    {
	 	    	
	 	    	
	 	    	getLogger().info("FRIConnectionQuality:" + channelInfo.getQuality()+"SessionState:" + channelInfo.getFRISessionState());
	 	    	
	 	    	
	 	    	FRIConnectionQuality c = null;
	 	    	 
	 	        if(c==FRIConnectionQuality.POOR)
	 	        {
	 	         getLogger().info("FRIConnectionQuality is POOR");
	 	         getLogger().info("Closing Session");
	 	          t=false;
	 	         }
	 	     }*/
			  
			
							
			    
				 
	 	}
	        
	
	public static void main(final String[] args)
    {
        final TestFRI app = new TestFRI();
        app.runApplication();
    }
}
