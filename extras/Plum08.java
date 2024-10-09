// Plum08
// 01-09-2000
// Gumowski-Mira-Martin-08, SCF

import java.awt.*;
import java.applet.*;
import java.awt.image.MemoryImageSource;
import java.awt.event.*;
import java.lang.Math;

public class Plum08 extends Applet implements Runnable
{
  int width, height;
  int i, ix, iy;
  int pixels[], buffer[];
  int cmask, color;
  int red, grn, blu;  
  double rFactor, gFactor, bFactor;    
  int tmp, nPoints, kicker;
  int nColorMode;
  int index;
  int mx;
  int my;
  //int red_tab[], grn_tab[], blu_tab[];
  int bug;
  
  double x, xs, y, ys, wx, a, b, c, sign, scale, tmp2, z, W;  
  double AA, BB, xn, yn;
  double LSum;

  Image image;
	MemoryImageSource source; 
  Thread thread;
  //Plum08 Plum08;
	//Frame f;
    
  public void init()
	{           
    width =   800;
    height =  600;        
		resize(width, height);

    //f = new Frame("Gumowski-Mira-Martin-01");        
    thread = null;
    
    ///////////////////////////
    
    //f.setSize(width, height+22);           
    //f.addWindowListener(new WindowCloser());
  	//f.add ("Center", this);
    //f.show();    
    //f.setTitle(" Iterations et Flarium24");          
    
    ///////////////////////////    

    pixels = new int[width*height];
    buffer = new int[width*height];
    
    //red_tab = new int[width*height];
    //grn_tab = new int[width*height];
    //blu_tab = new int[width*height];    
    
    for (i=0; i<width*height; i++)
    {
      pixels[i] = 0xff000000;
      buffer[i] = 64;
      //red_tab[i] = 0;      
      //grn_tab[i] = 0;      
      //blu_tab[i] = 0;      
    }
    
    source = new MemoryImageSource(width, height, pixels, 0, width);	  	  
    image = createImage(source);               
    nColorMode = 1;    
    cmask = 0xff000000;
    
    mx = width / 2;  
    my = height / 2;      
    
    bug = 0;
    scale = 7;    
    
    initXY();
        
  }

  /*
  public static void main (String args[]) 
  {
    init();        
    start();    
  }
  */
  
  public void initXY() 
  {    
  	tmp2 = width*height;

    for (i=0; i < tmp2; i++)
    {
    	// intialize image to black
      pixels[i] = 0xff000000;
      buffer[i] = 64;
    }
		   
		/*   
    for (i=0; i < tmp2; i++)
    {    
      red_tab[i] = 0;      
      grn_tab[i] = 0;      
      blu_tab[i] = 0;      
    }
    */
    
		RandomConstants();
	    
  }
  
  public void RandomConstants()
	{
		rFactor = 100*(Math.random()-0.5);
		gFactor = 100*(Math.random()-0.5);
		bFactor = 100*(Math.random()-0.5);
		
    a = (Math.random()-0.5);
    b = (Math.random()-0.5);
    c = (Math.random()-0.5);
    
    //AA = 0.5 + 0.2*(1+2*(Math.random()-0.5));
    AA = (Math.random()-0.5);
    BB = 1.00;      
        
    a = Math.abs(a);

    /*
    if (Math.abs(AA) < 0.5) 
  	{
  	  if (AA > 0)
  	  	AA = AA + 0.5;
  	  else
  	  	AA = AA - 0.5;	
   	}
   	*/
    
    b = a + 0.01;
    c = -1;
        
    x = -1.1;    
    y = 0;
    
    xs = 1;
    ys = 1;		
    
    //W = AA*x + sign*((1 - AA)*(2*x*x))/(1 + x*x);       
    W = 1;
    
    //f.setTitle("AA = " + AA);              
    
    nPoints = 0;
    kicker = 0;
		
	}
  
  public void paint(Graphics g)
  {        
    image.flush();
    g.drawImage(image, 0, 0, null);
  }    
  
  public void start() 
  {
  	if(thread == null) 
    {  
      thread = new Thread(this);
      thread.start();
    }
  }

  public void stop()
  {
    thread = null;
  }

  public void thread_manager()
  {
    repaint();      
		try 
    {
		  thread.sleep(100);
		} 
    catch( InterruptedException e ) 
    {
		}     
  }
  
  public void run() 
  {
	  thread = Thread.currentThread();
	  thread.setPriority(Thread.MIN_PRIORITY);             
	  while (thread != null) 
    {            
      initXY();                                         
      for (nColorMode = 1; nColorMode<= 50; nColorMode++)
      {                  		 
        // Modify the values in the pixels array at (x, y, w, h)                              
        for (i = 0; i < 30000; i++) 
        {            
          //x = x * nColorMode;
          //y = y * nColorMode;
                    
          nextGeneration();
          nPoints+=1;
          ix = (int) (scale*x + mx);
          iy = (int) (scale*y + my);
                      
          if ((Math.abs(scale*x) < mx*scale) && (Math.abs(scale*y) < my*scale) 
           && (Math.abs(xs - x) > 1e-6) && (Math.abs(ys - y) > 1e-6)) 
          {
            if ((Math.abs(scale*x) < mx) && (Math.abs(scale*y) < my))
          	{
              coloring();                     
              //if (sign > 0)
              pixels[ix + iy * (width)] = color;
          	}
          }
          else
        	{
          	bug = bug + 1;

         		try 
      			{
              if ((Math.abs(xs - x) < 1e-6) && (Math.abs(ys - y) < 1e-6))
      				{
		  				  thread.sleep(2);
								RandomConstants();
      				}
		  				else
      				{
		  				  thread.sleep(2000);
                initXY();						          			  				  
      				}
						} 
    				catch( InterruptedException e ) 
    				{
						}     						
          }          
        }
        thread_manager();
        coloring();      
      }
    }
  }

  public void nextGeneration() 
  { 
    if (nPoints % 100000 == 0)
  	{
      //AA = AA + 0.0001*Math.sin(AA);
      kicker = kicker + 1;
  	}
  	  	
    xs = x;
    ys = y;  	
    
    if (x >= 0)
      sign = 1;
    else
      sign = -1;    
  	
    //double wx = - Math.PI / 3.  * x + y + 20. * x / (x * x + 1.); // original       
    //double wx = - Math.PI / 2.8 * x + y + 100. * x / (x * x + 1.);            

    //wx = - Math.PI / 2.8 * x + y + 100. * x / (x * x + 2.);                
    //wx = - Math.PI / 3 * x + 1.01 * y + 50. * x / (x * x + 2);            
    
    /*
    if (nColorMode == 1)
      wx = -Math.PI / 3 * x + 1.001 * y + 80. * x / (x * x + y * y);                // plum01
    else
      wx =  Math.PI / 3 * x + 1.001 * y + 80. * x / (x * x + y * y);                // plum01
    */
    
    /*
    x = (x+1);
    y = (y+1);
    
    if (nColorMode == 1)
      wx = 1.2*x + y + 70 / (x*x - y*y - 1);
    else
      wx = -1.2*x - y - 70 / (x*x - y*y - 1);
    */
              
    //y = - x;
        
    //x    := x - (x * sign)/300;
    //xnew := v.af[2] * x + y + sign*sqrt(abs(sqrt(abs((x*(constPI*v.af[1]))))));
    //y    := v.af[3] - x;

    //x    = x - (x * sign)/300;
    //xn   = AA * x + y + sign*Math.sqrt(Math.abs(Math.sqrt(Math.abs((x*(Math.PI*AA))))));
      
    //xn   = y - sign*Math.sqrt(Math.abs(x*(AA+(Math.PI*x*AA))));
    //y    = 1 - x;
    //x    = xn;
    
    //x    = x - (x * sign)/200;
    //xn = a * x + y + sign * Math.sqrt(Math.abs(Math.sqrt(Math.abs(x*(Math.PI * b)))));
    //y  = c - x;
    //x    = xn;
    
    
    // Variation on a Barry Martin mapping

    //wx = a * x + y + sign * Math.sqrt(Math.abs(Math.sqrt(Math.abs(x*(Math.PI * b)))));
    //y  = c - x;
    
    //xn  = yn - sgn(xn)|sin(xn)cos(b) + c - xnsin(a + b + c)|
    //yn  = a - xn
    
    //wx  = y - sign * Math.sqrt(Math.abs(1 - x * a));
    //y   = - 1 - x;            
    //x = wx;
    
    //wx  = y + a * sign * (Math.sin(Math.PI*2*x));
    //y   = 1 - x;            
    //x = wx;

    //wx  = a * x + y - sign * Math.sqrt(Math.sqrt(Math.abs(x + b)));    
    //y   = c - x - 1;            
    //x = wx;
    
    
    // Gumowski-Mira-Martin    
    
    //z = x;
    //x = BB*y+W;

    // gm-03
    //W := (AA*x) + sign*((1 - AA)*(2*x*x))/(1+x*x);
    //W = sign*((AA*x) + (1 - AA)*(2*x*x))/(1 + x*x) + Math.sin(x);
    //W = sign*((AA*x) + (1 - AA)*(1*x*x))/(1 + x*x) + Math.sin(1/x);
    //y = W - z;
    
        
    // 9.)
    //xn =  BB * y + AA * x + 2 * x*x * (1-AA) / (1 + x*x);
    //yn = -x + AA * xn + AA * xn*xn * (1-AA) / (1 + xn*xn);    
    
    // 8
    x    = x + (x * sign)/50000;    
    xn =  BB * y+AA * x + 2*x*x * ((1 - AA) / (1 + x*x));
    y  = -x + AA*xn + 2*xn*xn * ((1 - AA) / (1 + xn*xn));        
    x  = xn;
    

/*
    if (Math.abs(x) <= 1e-4)
  	{
      x = 5*Math.cos(x);
  	}

    if (Math.abs(y) <= 1e-4)
  	{
      y = 5*Math.cos(y);
  	}

    if (Math.abs(x) > 1e2)
  	{
      x = 10+Math.sin(x);      
  	}
  	
    if (Math.abs(y) > 1e2)
  	{
      y = 10+Math.sin(y);
  	}      	    	    
*/
  	
  }
  
  void coloring()
  {
  	
  	/*
  	// from root-IFS_01
    LSum := v.dFactor1*abs(1e-6+sin(const2PI*arctan(y_save_advance/(1e-6+x_save_advance))));
    DrawRedPointArray;

    LSum := v.dFactor1*abs(1e-6+sin(const2PI*arctan(x/(1e-6+y))));
    DrawGrnPointArray;

    LSum := v.dFactor1*abs(1e-6+sin(const2PI*arctan(x_save_advance/(1e-6+y_save_advance))) -
                              sin(const2PI*arctan(x/(1e-6+y))));
    DrawBluPointArray;
  	*/  
  
    if ((Math.abs(scale*x) < mx) && (Math.abs(scale*y) < my))
  	{
  
    //LSum = 20*Math.abs(Math.sin(2*Math.PI*Math.atan(x/(1e-4+y)))+
    	//  Math.sin(2*Math.PI*Math.atan(xs/(1e-4+ys))));

    LSum = 20*Math.abs(Math.sin(Math.PI*Math.atan((1+x-xs)/(1+y-ys))));
      	
    index = ix + iy * (width);
   	buffer[index] = buffer[index] + (int) LSum;
    color = (int) (10*(Math.sqrt(10*buffer[index])));
    
  	if (color > 767)
  		color = 767;
      		  	
    if (color < 256)
      red = color;        
    else
      red = 255;
                      
    if (color < 512 && color > 255)
    {
      grn = color - 256;
    }
    else
    {
      if (color >= 512)
        grn = 255;
      else
        grn = 0;
    }
                                    
    if (color <= 768 && color > 511)
      blu = color - 512;
    else
  	{
  		if (color >= 768)
  			blu = 255;
  		else
        blu = 0;        
  	}

		tmp = (int)((red+grn+blu) * 0.33333);
		if (tmp > 255)
			tmp = 255;
			
		if (tmp < 0)
			tmp = 0;
			
		//red = tmp;
		//grn = tmp;
		//blu = tmp;

		red = tmp+(int)rFactor;
		grn = tmp+(int)gFactor;
		blu = tmp+(int)bFactor;

/*
		switch (kicker % 3)
  	{
  		case 0:			
		    red = tmp+(int)rFactor;
		    grn = tmp+(int)gFactor;
		    blu = tmp-(int)bFactor;
		    break;
		    
  		case 1:			
		    red = tmp-(int)gFactor;
		    grn = tmp+(int)bFactor;
		    blu = tmp-(int)rFactor;
		    break;

  		case 2:			
		    red = tmp+(int)bFactor;
		    grn = tmp-(int)rFactor;
		    blu = tmp+(int)gFactor;
		    break;
		    
  	}		    
  	  	
*/  	
  	
		    		
		if (red > 255)
			red = 255;
			
		if (red < 0)
			red = 0;

		if (grn > 255)
			grn = 255;
			
		if (grn < 0)
			grn = 0;

		if (blu > 255)
			blu = 255;
			
		if (blu < 0)
			blu = 0;
		
    
/*
      
	  if ((red & 0x1FF) > 0xFF) 
      red = red ^ 0xFF;    // Invert the color

    if ((grn & 0x1FF) > 0xFF) 
	  	grn = grn ^ 0xFF;    // Invert the color
          
    if ((blu & 0x1FF) > 0xFF) 
      blu = blu ^ 0xFF;    // Invert the color
      
*/
      
		/*
    // try swapping colors around
    //switch ((nColorMode & 3) + 1)
    switch ((int)sign)
    {
    case 11:
      break;
    case -1:
      tmp = red;
      red = grn;
      grn = blu;
      blu = tmp;
      break;
    case 1:
      tmp = grn;
      grn = blu;
      red = red;      
      blu = tmp;
      break;
    } 
    */     
               
    ////////////////////    
    
	  red = red & 0xFF;
	  grn = grn & 0xFF;
	  blu = blu & 0xFF;      

		/*    
    if (red > red_tab[index])
      red_tab[index] = (red + red_tab[index])/i;
    else
      red = red_tab[index];
    
    if (grn > grn_tab[index])
      grn_tab[index] = (grn + grn_tab[index])/i;
    else
      grn = grn_tab[index];
    
    if (blu > blu_tab[index])
      blu_tab[index] = (blu + blu_tab[index])/i;
    else
      blu = blu_tab[index];    

    */
        
    red = (red << 16) & 0x00ff0000;  // shift left 16 places            
    grn = (grn << 8)  & 0x0000ff00;  // shift left 8 places
        
    color = cmask | red | grn | blu;
        
    //iteration[index] = i;
    //pixels[index]    = cmask | color;    
    //index+=1;
    
  	}
  }
  
  public void update(Graphics g) 
  {
    paint(g);
  }
}

class WindowCloser extends WindowAdapter
{
  public void windowClosing(WindowEvent e)
  {
    Window win = e.getWindow();
    win.setVisible(false);
    win.dispose();
    //System.exit(0);        
  }
}