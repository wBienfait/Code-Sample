fAngle = 15; -- Angle max for the animation in Rad
fSpeed = 0.05; --Speed Rotation of the card in Rad
fOriginSpeed = 0.05; -- Original SpeedRotation in Rad
fSpeedUp = 0.01; --Acceleration Rad

function Move(Angle)
	--print('Move')
	fNewAngle = Angle + fSpeed;
	fSpeed = fSpeed + fSpeedUp;
	
	return fNewAngle;

end

function Check(Angle)
	--print('Check')
	if(Angle >= fAngle) then
		
		fSpeed = fOriginSpeed;
		return true;
	else
	
		return false;
		
	end

end

