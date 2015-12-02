using UnityEngine;
using System.Collections;
using UnityEngine.UI;


public class PlayerControl : MonoBehaviour
{

	//Useful objects
	Vector3 tInitialPos;
	Transform RightHand;
	public GameObject Ball = null;
	GameObject GameControle;
	Rigidbody Charac;
	public Text pUiOpponentScore;
	Material Mat;
	Animator tPlayerAnimator;

	//Score and life
	int iScore = 0;

	class BonusPattern
	{

        const float INFLUENCE_UP = 0.1f;
        float m_iInfluence;

        public BonusPattern()
        {
            m_iInfluence = 0.0f;

        }

        public void SetInput(int iCurve)
        {
            m_iInfluence += (INFLUENCE_UP * iCurve);
        }

        public float GetInput()
        {
            return m_iInfluence;
        }


        public void CleanInput()
        {
            m_iInfluence = 0.0f;
        } 

	};

	public int Score
	{
		get { return iScore; }
		set
		{
			iScore = value;
			UpdateScore();
		}
	}

	public int AnimValue
	{
		get { return tPlayerAnimator.GetInteger("Dir"); }
		set { tPlayerAnimator.SetInteger("Dir", value); }
	}

	//Player number 
	public int m_iPlayerId;
	public int MaxScore = 10;

	//Timers
	float RecupTime = 0.0f;
	float DodgeTimer = 0.0f;

	//Values to tweak
	public float CharacDrag = 25;
	public float Speed = 5;
	public float SmashTimer = 0.0f;
	public float MaxDodgeTime = 1.5f;
	public float GrabTimer = 0.0f;
	public float DodgeSpeed = 500;
	public float MaxRecupTime = 0.5f;
	public float MaxShootingTime = 0.5f;
	public float MaxGrabbingTime = 0.5f;
	//const float SmashingTime = 0.5f;

	public float AnimMinAngle = 45.0f;
	public float AnimMaxAngle = 120.0f;

	const int MaxLife = 2;

	public Vector3 HitDirection;

	bool bStart = false;
	bool bGoToInit = false;

	//input mode selector
	bool MOVE = true;
	bool SMASH = false;


	public enum EState
	{
		Active,
		Replayed
	}

	public enum EStatePlayerBall
	{
		Grab,
		Smash,
		Idle,
		Dead,
		Standby
	};

	public enum EStatePlayerMove
	{
		Run,
		RunWithBall,
		Dodging,
		Standby
	}

	public enum EPlayerInputType
	{
		A,//déplacement avec deux stick
		B,//déplacement face à la balle et quand on l’attrape on sais s'orienter librement.
		C//déplacement en se dirigeant avec un seul stick et quand on a la balle on sait s'orienter
	}

	public EStatePlayerBall eStateBall;
	public EStatePlayerMove eStateMove;
	private EPlayerInputType m_eInputType;
	private BonusPattern m_ePattern = new BonusPattern();

	public EPlayerInputType InputType//property of m_eInputType
	{
		get { return m_eInputType; }
		set
		{
			m_eInputType = value;
			print(m_eInputType);
			if (value == EPlayerInputType.B)//if rotation is ball LookAt
			{
				Ball = GameObject.FindGameObjectWithTag("Ball");//first ball found
				if (Ball == null)
					print("error : Ball is null. Input A is set");
			}
		}
	}

	private EState m_eState = EState.Active;

	public EState State
	{
		get { return m_eState; }
		set { m_eState = value; }
	}

	// Use this for initialization
	void Start()
	{
		//Refs
		RightHand = transform.FindChild("Root/Hips/Spine/Chest/RightShoulder/RightUpperArm/RightLowerArm/RightHand");
		Charac = transform.GetComponent<Rigidbody>();
		GameControle = GameObject.Find("GameControle");
		tPlayerAnimator = transform.GetComponent<Animator>();
		//transform.FindChild( "Viseur" ).FindChild( "Direction" ).FindChild( "Arrow" ).gameObject.SetActive( false );
		//Ball = null;

		//initializations
		tInitialPos = transform.position;
		Mat = transform.FindChild("Rys").GetComponent<Renderer>().material;
		Charac.drag = CharacDrag;
		eStateBall = EStatePlayerBall.Standby;
		eStateMove = EStatePlayerMove.Standby;
		m_eInputType = EPlayerInputType.B;
		tPlayerAnimator.SetInteger("Dir", 0);

		GrabTimer = MaxGrabbingTime;
        DodgeTimer = MaxDodgeTime;

		if (m_eInputType == EPlayerInputType.B && Ball == null)//tmp
			Ball = GameObject.FindGameObjectWithTag("Ball");
	}

	// Update is called once per frame 
    void FixedUpdate()
	{
		switch (m_eState)
		{
			case EState.Active:
				{
					if (RecupTime > 0.0f)
					{
						RecupTime -= Time.deltaTime;
						//print( RecupTime );
						if (RecupTime <= 0.0f)
						{
							RecupTime = 0.0f;

							IgnoreBallsCollisions(false);
						}
					}

					//----------------------------Movement----------------------------------------------------
					
                    MoveUpdate();

					//---------------------Behavior about ball-------------------------------------------------
					BallBehaviorUpdate();

					break;
				}

			case EState.Replayed:
				{

					break;
				}
		}
	}

	void MoveUpdate()
	{
		switch (eStateMove)
		{
			case EStatePlayerMove.Run:
				{
                    if (DodgeTimer < MaxDodgeTime)
                    {
                        DodgeTimer += Time.deltaTime;
                    }

		
					if (Input.GetButtonDown("Dodge_P" + m_iPlayerId))
					{
						eStateMove = EStatePlayerMove.Dodging;
                        eStateBall = EStatePlayerBall.Grab;
                        SmashTimer = MaxDodgeTime;
					}

                    if (RecupTime <= 0.0f)
                    {
                        Move(m_eInputType, MOVE);
                    }

					break;
				}
			case EStatePlayerMove.RunWithBall:
				{
					Move(m_eInputType, SMASH);
					break;
				}
			case EStatePlayerMove.Dodging:
				{
                    Dodge();
					break;
				}
			case EStatePlayerMove.Standby:
				{
					break;
				}
		}
	}

	void BallBehaviorUpdate()
	{
		switch (eStateBall)
		{
			case EStatePlayerBall.Idle:
				{
					if (Input.GetButtonDown("Shoot_P" + m_iPlayerId))
					{
						eStateBall = EStatePlayerBall.Grab;
						SmashTimer = MaxGrabbingTime;
						//Mat.color = Color.red;
					}

					if (Input.GetButton("Shield_P" + m_iPlayerId))
					{
						this.transform.FindChild("Shield").GetComponent<ShieldScript>().ShieldOn();
					}
					else
					{
						this.transform.FindChild("Shield").GetComponent<ShieldScript>().ShieldOff();
					}
					break;
				}
			case EStatePlayerBall.Grab:
				{
					SmashTimer -= Time.deltaTime;

					if (SmashTimer <= 0.0f)
					{
						//Mat.color = new Color(1, 1, 1);
						eStateBall = EStatePlayerBall.Idle;
						eStateMove = EStatePlayerMove.Run;
					}
					break;
				}
			case EStatePlayerBall.Smash:
				{
					//ChangeColor();
					GrabTimer -= Time.deltaTime;

					UpdateCurvePattern();
					eStateMove = EStatePlayerMove.RunWithBall;
					IgnoreBallsCollisions(true);
					Ball.transform.position = RightHand.position;

					if (GrabTimer <= 0.0f)
					{
						eStateBall = EStatePlayerBall.Idle;
						eStateMove = EStatePlayerMove.Run;

						if (Ball != null)
						{
							Ball.GetComponent<BallControl>().Launch(TestBallCurve());
							DesactivatePattern();
							RecupTime = MaxRecupTime;

							//if (m_eInputType != EPlayerInputType.B)//tmp
							//    Ball = null;
						}
						GrabTimer = MaxGrabbingTime;
					}
					break;
				}
			case EStatePlayerBall.Dead:
				{
					eStateMove = EStatePlayerMove.Standby;
                    
					if (iScore < MaxScore)
					{
						GameControle.GetComponent<GameControle>().RespawnGame();
					}
					else
					{
						GameControle.GetComponent<GameControle>().StopGame();
						print("All Team Dead");
					}
					break;
				}
			case EStatePlayerBall.Standby:
				{
					if (Input.GetButtonDown("Start_P" + m_iPlayerId))
					{
						bStart = true;
					}
					if (bStart)
					{
						GameControle.GetComponent<GameControle>().RestartGame();
					}
					break;
				}
		}
	}

    void UpdateCurvePattern()
    {

        int DirX = (int)(HitDirection.x / Mathf.Abs(HitDirection.x));
        if (Input.GetButton("ButtonX_P" + m_iPlayerId))
        {
            m_ePattern.SetInput(-DirX);
        }

        if (Input.GetButton("ButtonY_P" + m_iPlayerId))
        {
            m_ePattern.SetInput(DirX);
        }

    }
    float TestBallCurve()
    {
        return m_ePattern.GetInput();
    }
    void DesactivatePattern()
    {
        m_ePattern.CleanInput();
    }

	void Dodge()
	{
        DodgeTimer -= Time.deltaTime;

        Vector3 tDir = Vector3.zero;
        //translate
        float AxeX = Input.GetAxis("Horizontal_P" + m_iPlayerId);
        float AxeY = Input.GetAxis("Vertical_P" + m_iPlayerId);
        tDir = new Vector3(AxeX, 0, AxeY);

        tDir += HitDirection;
        tDir.Normalize();

        AnimUpdate(tDir);
        Charac.AddForce(tDir * ((DodgeTimer/MaxDodgeTime) * DodgeSpeed), ForceMode.Impulse);

        //Move(m_eInputType, MOVE);

        if (DodgeTimer <= 0.0f)
        {
            DodgeTimer = 0.0f;
            eStateMove = EStatePlayerMove.Run;
        }
	}

	void ChangeColor()
	{
		if (m_iPlayerId == 1)
		{
			Mat.color = new Color(0, 0, 1.0f);
		}
		else
		{
			Mat.color = new Color(1, 0.5f, 0.5f);
		}
	}

	void UpdateDirection()
	{

		float RightAxeX = Input.GetAxis("Horizontal_P" + m_iPlayerId);
		float RightAxeY = -Input.GetAxis("Vertical_P" + m_iPlayerId);

		if (RightAxeX != 0 || RightAxeY != 0)
		{
			HitDirection = new Vector3(RightAxeX, 0, -RightAxeY).normalized;

			transform.FindChild("Viseur").rotation = Quaternion.Euler(new Vector3(0, (Mathf.Atan2(RightAxeY, RightAxeX)) / Mathf.PI * 180, 0));
		}
		else
		{
			if (HitDirection == Vector3.zero)
			{
				HitDirection = transform.forward;
			}
		}
	}

	public void IgnoreBallsCollisions(bool bState)
	{
		GameObject[] Balls = GameObject.FindGameObjectsWithTag("Ball");

		for (int i = 0; i < Balls.Length; ++i)
		{
			Physics.IgnoreCollision(Balls[i].GetComponent<CapsuleCollider>(), GetComponent<Collider>(), bState);
		}
	}

	void OnCollisionEnter(Collision Col)
	{
		if (Col.transform.CompareTag("Ball") && eStateBall != EStatePlayerBall.Grab)
		{
			//iLife--;

			IgnoreBallsCollisions(true);
			RecupTime = MaxRecupTime * 1.5f;

			Vector3 HitDirection = Col.collider.GetComponent<BallControl>().dir;

			Charac.AddForce(HitDirection * Speed / 2, ForceMode.Impulse);

            if(Col.gameObject.GetComponent<BallControl>().IsOffensive(m_iPlayerId))
            {
                StunMode();
            }


			//Update life in UI

			//launch actual ball if is grabbing

		}
	}

    void StunMode()
    {
        print("BOOOOOOOOOOOOOM");
        RecupTime = MaxRecupTime * 3.0f;
        Charac.AddForce(HitDirection * Speed / 2, ForceMode.Impulse);
    }

	public void IncrementScore()
	{
		iScore++;
	}


	public void UpdateScore()
	{
		pUiOpponentScore.text = iScore.ToString();
	}

	public void Respawn()
	{
        if (Ball != null)
        {
            Ball.GetComponent<BallControl>().Launch(TestBallCurve());
        }
		eStateMove = EStatePlayerMove.Run;
		eStateBall = EStatePlayerBall.Idle;
		Charac.useGravity = false;
		bStart = false;
		this.transform.FindChild("Shield").GetComponent<ShieldScript>().ResetShield();
		Charac.constraints = RigidbodyConstraints.FreezeRotation | RigidbodyConstraints.FreezePositionY;
		Charac.drag = CharacDrag;
		IgnoreBallsCollisions(false);
	}

	public void Restart()
	{
		eStateMove = EStatePlayerMove.Run;
		eStateBall = EStatePlayerBall.Idle;
		Charac.useGravity = false;
		iScore = 0;
		bStart = false;
		this.transform.FindChild("Shield").GetComponent<ShieldScript>().ResetShield();
		Charac.constraints = RigidbodyConstraints.FreezeRotation | RigidbodyConstraints.FreezePositionY;
		Charac.drag = CharacDrag;
		tPlayerAnimator.SetInteger("Dir", 0);

		IgnoreBallsCollisions(false);
	}

	public bool GoToInitialPos()
	{
		bGoToInit = true;
		this.transform.rotation = Quaternion.Euler(Vector3.zero);
		this.transform.position = Vector3.Lerp(this.transform.position, tInitialPos, Time.deltaTime);
		Vector3 Distance = tInitialPos - this.transform.position;
		if (Distance.magnitude <= 1)
		{
			this.transform.position = tInitialPos;
			bGoToInit = false;
			return true;
		}
		return false;
	}

	public void InitSmash()
	{
		eStateBall = EStatePlayerBall.Smash;
		SmashTimer = MaxShootingTime;
	}

	public void UpdateMaxShoot(float maxShootingTime)
	{
		MaxShootingTime = maxShootingTime;
	}

	public void UpdateMaxGrab(float maxGrabbingTime)
	{
		MaxGrabbingTime = maxGrabbingTime;
	}

	void AnimUpdate(Vector3 tDir)
	{
		if (eStateBall != EStatePlayerBall.Smash)
		{
			if (Ball != null)
			{
				//P2B == Player to Ball
				Vector3 tBallPos = Ball.transform.position;
				Vector3 tPlayerPos = transform.position;
				Vector3 tP2BDir = tBallPos - tPlayerPos;

				float fP2BAngle = Vector2.Angle(new Vector2(tP2BDir.x, tP2BDir.z), new Vector2(tDir.x, tDir.z));
				Vector3 fP2BAngle2 = Vector3.Cross(tP2BDir, tDir);

				if (fP2BAngle2.y < 0)
					fP2BAngle = -fP2BAngle;

				if (fP2BAngle > 0)
				{
					if (fP2BAngle < AnimMinAngle &&
						tPlayerAnimator.GetInteger("Dir") != 1)//forward
					{
						tPlayerAnimator.SetInteger("Dir", 1);
						//print( "forward" );
					}
					if (fP2BAngle > AnimMinAngle &&
					   fP2BAngle < AnimMaxAngle &&
					   tPlayerAnimator.GetInteger("Dir") != 3)//right
					{
						tPlayerAnimator.SetInteger("Dir", 3);
						//print( "right" );
					}
					if (fP2BAngle > AnimMaxAngle &&
					   tPlayerAnimator.GetInteger("Dir") != -1)//backward
					{
						tPlayerAnimator.SetInteger("Dir", -1);
						//print( "backward" );
					}
					if (fP2BAngle == 90.0f &&
					   tPlayerAnimator.GetInteger("Dir") != 0)
					{
						tPlayerAnimator.SetInteger("Dir", 0);
					}
				}
				else
				{
					if (fP2BAngle > -AnimMinAngle &&
						tPlayerAnimator.GetInteger("Dir") != 1)//forward
					{
						tPlayerAnimator.SetInteger("Dir", 1);
						//print( "forward" );
					}
					if (fP2BAngle < -AnimMinAngle &&
					   fP2BAngle > -AnimMaxAngle &&
					   tPlayerAnimator.GetInteger("Dir") != 2)//left
					{
						tPlayerAnimator.SetInteger("Dir", 2);
						//print( "left" );
					}
					if (fP2BAngle < -AnimMaxAngle &&
					   tPlayerAnimator.GetInteger("Dir") != -1)//backward
					{
						tPlayerAnimator.SetInteger("Dir", -1);
						//print( "backward" );
					}
					if (fP2BAngle == 90.0f &&
					   tPlayerAnimator.GetInteger("Dir") != 0)
					{
						tPlayerAnimator.SetInteger("Dir", 0);
					}
				}
			}
		}
		else
		{
			if (tPlayerAnimator.GetInteger("Dir") != 4)
			{
				tPlayerAnimator.SetInteger("Dir", 4);
			}
		}
	}

	void Move(EPlayerInputType eInputType, bool bMode)
	{
		if (bGoToInit == false)
		{
			Vector3 tDir = Vector3.zero;
			//translate
			float AxeX = Input.GetAxis("Horizontal_P" + m_iPlayerId);
			float AxeY = Input.GetAxis("Vertical_P" + m_iPlayerId);
			tDir = new Vector3(AxeX, 0, AxeY);

			//anim settup
			UpdateDirection();

			if (bMode == MOVE)
			{
				AnimUpdate(tDir);
				Charac.AddForce(tDir * Speed * Time.deltaTime, ForceMode.Impulse);
			}
			else
			{
				AnimUpdate( tDir );

				Charac.velocity = new Vector3(0, 0, 0);
			}

			//rotate
			switch (eInputType)
			{
				case EPlayerInputType.A:
					{
						RightAxisRotation();
						break;
					}
				case EPlayerInputType.B:
					{
						if (bMode == MOVE)
						{
							if (tPlayerAnimator.GetInteger("Dir") != 0)
								Charac.transform.LookAt(new Vector3(Ball.transform.position.x, 0, Ball.transform.position.z));
						}
						else
							RightAxisRotation();
						break;
					}
				case EPlayerInputType.C:
					{
						if (bMode == MOVE)
							Charac.transform.LookAt(new Vector3(Ball.transform.position.x, 0, Ball.transform.position.z));
						else
							RightAxisRotation();
						break;
					}
			}
		}
	}

	public void RightAxisRotation()
	{
		float RightAxeX = Input.GetAxis("RightHorizontal_P" + m_iPlayerId);
		float RightAxeY = Input.GetAxis("RightVertical_P" + m_iPlayerId);

		if (RightAxeX != 0 || RightAxeY != 0)
		{
			Charac.transform.localRotation = Quaternion.Euler(new Vector3(0, (Mathf.Atan2(RightAxeY, RightAxeX) + Mathf.PI / 2) / Mathf.PI * 180, 0));
		}
	}

	public void UpdateInputMode(int eInputType)
	{
		InputType = (EPlayerInputType)(eInputType);
	}
}
