void gearToMatchSpeed()
{
    using namespace geartomatchspeed;

    // Theoretical RPM according to speed

    firstGearRPMSpeedRatio = cansignal::vehicleSpeed / ((14/35) * (36/79) * 0.4 * wheelRadius * 0.0254 * ((2*3.14159265359)/60) * 3.6);
    secondGearRPMSpeedRatio = cansignal::vehicleSpeed / ((16/28) * (36/79) * 0.4 * wheelRadius * 0.0254 * ((2*3.14159265359)/60) * 3.6);
    thridGearRPMSpeedRatio = cansignal::vehicleSpeed / ((21/28) * (36/79) * 0.4 * wheelRadius * 0.0254 * ((2*3.14159265359)/60) * 3.6);
    fourthGearRPMSpeedRatio = cansignal::vehicleSpeed / ((21/23) * (36/79) * 0.4 * wheelRadius * 0.0254 * ((2*3.14159265359)/60) * 3.6);
    fifthGearRPMSpeedRatio = cansignal::vehicleSpeed / ((23/22) * (36/79) * 0.4 * wheelRadius * 0.0254 * ((2*3.14159265359)/60) * 3.6);
    sixthGearRPMSpeedRatio = cansignal::vehicleSpeed / ((23/20) * (36/79) * 0.4 * wheelRadius * 0.0254 * ((2*3.14159265359)/60) * 3.6);
    
    



    

    if (gearposition::currentGear == 1 && firstGearRPMSpeedRatio > 4000 && firstGearRPMSpeedRatio < 7000 )
    {
        clutchInCheck = 1;
    }
    else if (gearposition::currentGear == 2 && secondGearRPMSpeedRatio > 4000 && secondGearRPMSpeedRatio < 7000 )
    {
        clutchInCheck = 1;
    }
    else if (gearposition::currentGear == 3 && thirdGearRPMSpeedRatio > 4000 && thirdGearRPMSpeedRatio < 7000 )
    {
        clutchInCheck = 1;
    }
    else if (gearposition::currentGear == 4 && fourthGearRPMSpeedRatio > 4000 && fourthGearRPMSpeedRatio < 7000 )
    {
        clutchInCheck = 1;
    }
    else if (gearposition::currentGear == 5 && fifthGearRPMSpeedRatio > 4000 && fifthGearRPMSpeedRatio < 7000 )
    {
        clutchInCheck = 1;
    }
    else if (gearposition::currentGear == 6 && sixthGearRPMSpeedRatio > 4000 && sixthGearRPMSpeedRatio < 7000 )
    {
        clutchInCheck = 1;
    }
    else
    {
        clutchInCheck = 0;   
    }
    
}