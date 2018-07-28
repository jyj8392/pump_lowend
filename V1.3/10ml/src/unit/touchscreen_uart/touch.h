struct button_press {
  	u8 freeze;
  	u8 thaw;
  	u8 stick;
	u8 unlock;
	u8 bleeze;
	u8 stopall;
};

struct response {
  	u8 cmd;
	u16 addr;
	u8 len;
	u16 data;
};

#define SetPower0Start	((452 << 16) + 91			   )
#define SetPower0End	(SetPower0Start + (70 << 16) + 40)
#define SetPower1Start	((452 << 16) + 150			   )
#define SetPower1End	(SetPower1Start + (70 << 16) + 40)
#define SetPower2Start	((452 << 16) + 209			   )
#define SetPower2End	(SetPower2Start + (70 << 16) + 40)
#define SetPower3Start	((452 << 16) + 268			   )
#define SetPower3End	(SetPower3Start + (70 << 16) + 40)

#define Freeze0Start	((661 << 16) + 85			   )
#define Freeze0End		(Freeze0Start + (65 << 16) + 51) 
#define Freeze1Start	((661 << 16) + 145			   )
#define Freeze1End		(Freeze1Start + (65 << 16) + 51)
#define Freeze2Start	((661 << 16) + 204		  	   )
#define Freeze2End 		(Freeze2Start + (65 << 16) + 51)
#define Freeze3Start	((661 << 16) + 263		  	   )
#define Freeze3End 		(Freeze3Start + (65 << 16) + 51)

#define Thaw0Start		((738 << 16) + 85			   )
#define Thaw0End 		(Thaw0Start + (65 << 16) + 51  )
#define Thaw1Start		((738 << 16) + 145			   )
#define Thaw1End 	    (Thaw1Start + (65 << 16) + 51  )
#define Thaw2Start		((738 << 16) + 204			   )
#define Thaw2End 		(Thaw2Start + (65 << 16) + 51  )
#define Thaw3Start		((738 << 16) + 263			   )
#define Thaw3End 		(Thaw3Start + (65 << 16) + 51  )

#define Stick0Start		((585 << 16) + 85			   )
#define Stick0End 		(Stick0Start + (65 << 16) + 51 )
#define Stick1Start		((585 << 16) + 145			   )
#define Stick1End 		(Stick1Start + (65 << 16) + 51 )
#define Stick2Start		((585 << 16) + 204			   )
#define Stick2End 		(Stick2Start + (65 << 16) + 51 )
#define Stick3Start		((585 << 16) + 263			   )
#define Stick3End 		(Stick3Start + (65 << 16) + 51 )

#define BleezeStart		((901 << 16) + 174			   )
#define BleezeEnd 		(BleezeStart + (95 << 16) + 55 )

#define GeiLiStart		((376 << 16) + 692			   )
#define GeiLiAdj(x) 	(GeiLiStart + (x << 16) + 42   )
#define GeiLiEnd 		(GeiLiStart + (604 << 16) + 42 )
