/* ---------- T I M E . H --------- */


#ifndef _TIME_H_
#define _TIME_H_




class time {
	public:
		time time();							/* TIME DEFAULT CONSTRUCTOR */
		
		year Get_Year(){ return yrYears; }		/* TIME DEFAULT CONSTRUCTOR */
		month Get_Month(){ return monMonths; }	
		day Get_Day(){ return dayDays; }	
		hour Get_Hour(){ return hrHours; }	
		minute Get_Minute(){ return mntMinutes; }	
		second Get_Second(){ return scdSeconds; }	
		period Get_Period(){ return prdAMPM; }	

		void Set_Time(year yrToBeSet, month monToBeSet, day dayToBeSet, hour hrToBeSet, minute mntToBeSet, second scdToBeSet, period prdToBeSet);	/* TIME MUTATOR FUNCTIONS */
		void Set_Year(year yrToBeSet);
		void Set_Month(month monToBeSet);
		void Set_Day(day dayToBeSet);
		void Set_Hour(hour hrToBeSet);
		void Set_Minute(minute mntToBeSet);
		void Set_Second(second scdToBeSet);
		void Set_Period(period prdToBeSet);

	private:
		year yrYears;							/* DATA MEMBERS */
		month monMonths;
		day dayDays;
		hour hrHours;
		minute mntMinutes;
		second scdSeconds;
		period prdAMPM;
};


#endif
