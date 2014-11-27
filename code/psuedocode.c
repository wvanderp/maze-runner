/* Code for activity bot v0.4 */
//psuedo code v0.4
//in deze method heb je de "keep drive forward" en een if statement, als de ultrasound detecteert dat de afstand equal is aan 7 cm wordt de method check aangeroepen.
 methode drive{
 
	if{ 7 cm )
	{
		call method check;
	}
	
	else 
	{
		call method drive;
	}
 
 }
 
 //infra-red, this will be used as a boolean type "true or false" to check both sides and from there it will make a decision and turn from there it will turn 90 degrees.
 method check{
	
	if( left == true && right != true ) //if left is true and only left
	{
		//turn 90 degrees to the right
	}
	
	else if( right == true && left != true ) //if right is true and only right
	{
		//turn 90 degrees to the left
	}
	
	else if( left && right == true ) //if left and right are true
	{
		//turn 180 degrees
	}
	
	//left is chosen because nor left and right is true. In other words there is not a wall on each side. Left will be defined as standard in this case.
	else
	{
		//turn 90 degrees to the left
	}
	
	call methode drive;
 }

}