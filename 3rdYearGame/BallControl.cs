using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public class BallControl : MonoBehaviour {


    public enum EState
    {
        Active,
        Replayed
    }

	public float Speed = 0.5f;
	public float WallImpulse = 2;
    public Text pUiSpeed = null;
    public float SpeedIncrement = 5.0f;
    public float DownSpeed = 0.5f;
	public Vector3 dir = Vector3.zero;

    //---Curve Initialization---------------
    Vector3 BonusDir = Vector3.zero;
    float BonusDirInfluence = 0.0f;
    float BonusDirCounterInfluence = 0.0f;
    public float DownInfluence = 0.1f;
    float DownInfluenceFact = 0.0f;

    private float MaxRadiusDistance = 25.0f;//Circle out of which the ball can move
    public int Rotationspeed = 60;

    float fOffensiveTimer = 0.0f;
    public float MaxTimerOffensive = 3.0f;


    public bool IsPicked = false;
    public bool IsGrabed = false;
    bool IsDangerous = false;
    int iPlayerSafe = 0;

    public float MaxSpeed = 100.0f;
    public float MinSpeed = 10.0f;

    Rigidbody Rb;
	public ParticleSystem Sparks = null;
    //GameObject ActiveBonus;
    public int LastPlayerID;

    public EState m_eState = EState.Active;

    public EState State
    {
        get { return m_eState; }
        set { m_eState = value; }
    }

// Use this for initialization
	public virtual void Start () {
        Rb = GetComponent<Rigidbody>();
        m_eState = EState.Active;

		//if (Speed < MinSpeed)
		//    Speed = MinSpeed;
		Speed = MinSpeed;
		SpeedIncrement = (1.0f / (Speed / 100.0f)) * 2.0f;

        UpdateSpeedText();
	}
	
	// Update is called once per frame
    public virtual void FixedUpdate()
    {
        if (m_eState == EState.Active)
        {
            ChangeVelocity();
            if(fOffensiveTimer >= 0.0f)
            {
                fOffensiveTimer -= Time.deltaTime;
                if(fOffensiveTimer <= 0.0f)
                {
                    IsDangerous = false;
                    iPlayerSafe = 0;
                }
            }

            //Rb.rotation *= (Quaternion.AngleAxis(Time.deltaTime * Rotationspeed * Speed, Vector3.up));
            //print(Rb.rotation.eulerAngles);

            if (IsGrabed)
            {
                transform.position = transform.parent.FindChild("Viseur").FindChild("Direction").transform.position;
            }

            if (Rb.position.x > MaxRadiusDistance ||
                Rb.position.z > MaxRadiusDistance ||
                Rb.position.x < -MaxRadiusDistance ||
                Rb.position.z < -MaxRadiusDistance)
            {
                Rb.position = new Vector3(0, 1.28f, 0);
            }
        }
	}

	public void GetGrabed()
	{
		IsGrabed = true;
		Rb.isKinematic = true;

		//set new dir (player's dir)
		LastPlayerID = transform.parent.GetComponent<PlayerControl>().m_iPlayerId;
		dir = transform.parent.GetComponent<PlayerControl>().HitDirection;
		dir.y = 0;
		//set pos to player
		transform.localPosition = transform.parent.GetComponent<PlayerControl>().HitDirection * 2;
	}

    public void SetOffensive()
    {
        IsDangerous = true;
        fOffensiveTimer = MaxTimerOffensive;
        iPlayerSafe = LastPlayerID;
    }

    public bool IsOffensive(int Player)
    {
        if(IsDangerous && Player != iPlayerSafe)
        {
            return true;
        }
        return false;
    }

    public void Launch(float fCurve)
    {
        BonusDir = Vector3.zero;
        BonusDirInfluence = 0.0f;
		if( transform.parent != null )
		{
			dir = transform.parent.transform.gameObject.GetComponent<PlayerControl>().HitDirection;
            float fAngle = (Mathf.PI / 2) * (fCurve / Mathf.Abs(fCurve));
            BonusDir = new Vector3(dir.x * Mathf.Cos(fAngle) - dir.z * Mathf.Sin(fAngle), 0.0f, dir.x * Mathf.Sin(fAngle) - dir.z * Mathf.Cos(fAngle));
            BonusDirCounterInfluence = Mathf.Abs(fCurve);
            BonusDirInfluence = BonusDirCounterInfluence * 2;
            DownInfluenceFact = BonusDirCounterInfluence;
		}
        ChangeColor();
        transform.parent = null;
        IsPicked = false;
		IsGrabed = false;
        Rb.isKinematic = false;
        
        if(fCurve == 0)
            SpeedUp();
        
		/*
        if (Speed < MaxSpeed)
        {
            Speed += SpeedIncrement;
            UpdateSpeedText();
        }
		*/
	}

	public void ChangeVelocity()
	{
        Vector3 FinalDir = dir;
        
        if (BonusDirInfluence != 0.0f)
        {
            FinalDir = dir + (BonusDir * (BonusDirInfluence - BonusDirCounterInfluence));

            BonusDirInfluence -= DownInfluence * DownInfluenceFact;
            if(Mathf.Abs(BonusDirInfluence) < DownInfluence)
            {
                dir = FinalDir;
                BonusDirInfluence = 0.0f;
            }
        }
        //else if(BonusDirCounterInfluence > 0.0f)
        //{
        //    FinalDir = dir + (BonusDir * -BonusDirCounterInfluence);
        //    BonusDirCounterInfluence -= DownInfluence;
        //    if (Mathf.Abs(BonusDirCounterInfluence) < DownInfluence)
        //    {
        //        BonusDirInfluence = 0.0f;
        //    }
        //}

        FinalDir.Normalize();
		
        Rb.velocity = FinalDir * Speed;
		
	}

	void ChangeColor()
    {
        if (Speed < MaxSpeed)
		{
			if ( transform.parent != null )
			{
				int iId = this.transform.parent.GetComponent<PlayerControl>().m_iPlayerId;

				if (iId == 1)
				{
					GetComponent<Renderer>().material.color = new Color(0, 0, 1.0f);
				}
				else if (iId == 2)
				{
					GetComponent<Renderer>().material.color = new Color(1, 0.5f, 0.5f);
				}
			}
        }
        else
        {
            print("Full SPeed");
            //this.transform.GetComponent<MeshRenderer>().material.color = new Color(1.0f, 0.0f, 0.0f); 
            GetComponent<Renderer>().material.color = new Color(1.0f ,0.0f, 0.0f); 
        }
    }
	
	public void SpeedUp()
    {
        if (Speed < MaxSpeed)
        {
            Speed += SpeedIncrement;
			SpeedIncrement = (1.0f / (Speed / 100.0f)) * 2.0f;
            UpdateSpeedText();
        }
    }

    public void SpeedDown()
    {
        if (Speed > MinSpeed)
        {
            Speed -= DownSpeed;
            UpdateSpeedText();
        }
    }
	
	void CollisionBehavior(Collision Col)
	{
		if (Col.transform.CompareTag("Terrain") || Col.transform.CompareTag("Bumper") || Col.transform.CompareTag("Goal"))
		{
			ContactPoint Contact = Col.contacts[0];
			Vector3 normal = Contact.normal;

			if (m_eState == EState.Active)
			{
				dir = Vector3.Reflect(dir, normal);
				dir.y = 0;
				SpeedDown();
				BonusDir = Vector3.zero;
				BonusDirInfluence = 0.0f;
			}

			if (Sparks != null)
				Instantiate(Sparks, Contact.point, Quaternion.Euler(normal));
			//NewSparks.transform.parent = transform;
		}

		if (Col.transform.CompareTag("Player"))
		{
            
			if (m_eState == EState.Active)
			{
                float fAngle = Random.Range(-Mathf.PI/2, Mathf.PI/2);
                dir = new Vector3(dir.x * Mathf.Cos(fAngle) - dir.z * Mathf.Sin(fAngle), 0.0f, dir.x * Mathf.Sin(fAngle) - dir.z * Mathf.Cos(fAngle));
			}
			//if (Sparks != null)
			//    Instantiate(Sparks, Contact.point, Quaternion.Euler(normal));
			//NewSparks.transform.parent = transform;
		}
	}

    public virtual void OnCollisionEnter(Collision Col)
	{
		CollisionBehavior(Col);
	}

	public virtual void OnCollisionStay(Collision Col)
	{
		CollisionBehavior(Col);
	}

    //void OnTriggerEnter(Collider Col)
    //{
    //    if (Col.transform.CompareTag("Bonus") && Col.GetComponent<PipeBonus>().isUp)
    //    {
    //        ActiveBonus = Col.gameObject;
    //        print(ActiveBonus);
    //    }
    //}

    void OnCollisionExit(Collision Col)
    {
        this.transform.position = new Vector3(this.transform.position.x, 1.7f, this.transform.position.z);
		transform.rotation = Quaternion.identity;
    }

    void UpdateSpeedText()
    {
		if(pUiSpeed != null)
			pUiSpeed.text = "Ball Speed : " + Speed.ToString();
    }

    public void Stop()
    {
        Speed = MinSpeed;
        dir = Vector3.zero;
        this.transform.position = new Vector3(0.0f, this.transform.position.y, 0.0f);
    }

    public void Restart()
    {
        Speed = MinSpeed;
        LastPlayerID = 0;
        //float fAngle = Random.Range(0.0f, (2 * Mathf.PI));
        dir = Vector3.zero;
        //dir = new Vector3(-Mathf.Sin(fAngle), 0.0f, Mathf.Cos(fAngle));
        this.transform.position = new Vector3(0.0f, this.transform.position.y, 0.0f);
    }
}
