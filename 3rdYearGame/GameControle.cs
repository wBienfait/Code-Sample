using UnityEngine;
using System.Collections;
using UnityEngine.UI;


public class GameControle : MonoBehaviour
{
    enum EMode
    {
        Menu,
        Gameplay,
        Replay,
        Transition
    }
    enum ETransitionTo
    {
        Game,
        Replay
    }

    GameObject[] m_tPlayers;
    GameObject m_tBall;
	GameObject m_tTerrain;
    GameObject[] m_tGoals;
    ReplayManager m_ReplayManager = null;
    public Text pUiStartCount;
    public Text pUiReplay;
    GameObject[] pUiTransition;

    int iLastGoalID = 0;
    bool bTransition = false;
    float TimerBeforeStart;
    public float MaxTimer = 3.0f;
    public float TimerWhenGoal;
    public float MaxTimerGoal = 1.0f;

    EMode m_eMode = EMode.Gameplay;
    ETransitionTo m_eTransitionTo = ETransitionTo.Game;

	//EMode Mode
	//{
	//	get { return m_eMode; }
	//	set
	//	{
	//		m_eMode = value;
	//		if (value == EMode.Replay)
	//			m_ReplayManager.StartReplay();
	//	}
    //
	//}

    void Start()
    {
        m_tPlayers = GameObject.FindGameObjectsWithTag("Player");
        m_tBall = GameObject.FindGameObjectWithTag("Ball");
		m_tTerrain = GameObject.Find("Stade");
        m_tGoals = GameObject.FindGameObjectsWithTag("Goal");
        pUiTransition = GameObject.FindGameObjectsWithTag("Transition");
        m_ReplayManager = transform.GetComponentInChildren<ReplayManager>();

        m_eMode = EMode.Gameplay;
        TimerBeforeStart = MaxTimer;
    }

    void Update()
    {
        
        switch (m_eMode)
        {
            case EMode.Gameplay:
                {

                    m_ReplayManager.Record();
                    break;
                }
            case EMode.Replay:
                {
                    if (!bTransition)
                    {
                        int iTransition = pUiTransition.Length;
                        if (!m_ReplayManager.Play())
                        {
                            bTransition = true;

                            for (int i = 0; i < iTransition; i++)
                            {
                                pUiTransition[i].GetComponent<FadeTransition>().Transition();
                            }

                        }
                        int iPlayer = m_tPlayers.Length;
                        for (int i = 0; i < iPlayer; i++)
                        {
                            if (Input.GetButtonDown("Start_P" + (i + 1)))
                            {

                                for (int j = 0; j < iTransition; j++)
                                {
                                    pUiTransition[j].GetComponent<FadeTransition>().Transition();
                                }
                                bTransition = true;
                                break;
                            }

                        }
                    }
                    else
                    {
                        int iTransition = pUiTransition.Length;
                        for (int i = 0; i < iTransition; i++)
                        {
                            if (pUiTransition[i].GetComponent<FadeTransition>().TimeToChange())
                            {
                                print("Return to game");
                                pUiReplay.enabled = false;
                                m_eMode = EMode.Gameplay;
                                pUiReplay.enabled = false;
                                m_ReplayManager.StopReplay();
                                bTransition = false;
                                break;
                            }
                        }
                    }
                   
                    break;
                }
            case EMode.Transition:
                {

                    TimerWhenGoal -= Time.deltaTime;
                    if (TimerWhenGoal <= 0.0f)
                    {
                        int iTransition = pUiTransition.Length;
                        
                        for (int i = 0; i < iTransition; i++)
                        {
                            pUiTransition[i].GetComponent<FadeTransition>().Transition();
                            if(pUiTransition[i].GetComponent<FadeTransition>().TimeToChange())
                            {
                                m_eMode = EMode.Replay;
                                m_ReplayManager.StartReplay(iLastGoalID);
                                pUiReplay.enabled = true;
                                break;
                            }
                        }
                        
                    }
                    else
                    {
                        m_ReplayManager.Record();
                    }
                    
                              
                    break;
                }
            case EMode.Menu:
                {

                    break;
                }
        }
    }

    public void TransitionToReplay(int iGoal)
    {
        m_eMode = EMode.Transition;
        TimerWhenGoal = MaxTimerGoal;
        iLastGoalID = iGoal;
    }

    public void RestartGame()
    {
        bool bInitPos = true;
        for (int i = 0; i < m_tPlayers.Length; ++i)
        {
            if (m_tPlayers[i].GetComponent<PlayerControl>().GoToInitialPos() != true)
            {
                bInitPos = false;
            }
        }

        if (bInitPos)
        {
            TimerBeforeStart -= Time.deltaTime;
            UpdateStartCountdown();

            if (TimerBeforeStart <= 0.0f)
            {

                for (int i = 0; i < m_tPlayers.Length; ++i)
                {
                    m_tGoals[i].SetActive(true);
                    m_tPlayers[i].GetComponent<PlayerControl>().Restart();
                    m_tBall.GetComponent<BallControl>().Restart();
                }
                TimerBeforeStart = MaxTimer;
            }
        }
    }

    public void StopGame()
    {
        for (int i = 0; i < m_tPlayers.Length; ++i)
        {
            m_tPlayers[i].GetComponent<PlayerControl>().eStateBall = PlayerControl.EStatePlayerBall.Standby;
            m_tBall.GetComponent<BallControl>().Stop();
        }
    }

    public void RespawnGame()
    {
        if (m_eMode == EMode.Gameplay)
        {
            bool bInitPos = true;
            for (int i = 0; i < m_tPlayers.Length; ++i)
            {

                if (m_tPlayers[i].GetComponent<PlayerControl>().GoToInitialPos() != true)
                {
                    bInitPos = false;
                }
            }
            m_tBall.GetComponent<BallControl>().Stop();

            if (bInitPos)
            {
                TimerBeforeStart -= Time.deltaTime;
                UpdateStartCountdown();

                if (TimerBeforeStart <= 0.0f)
                {
                    for (int i = 0; i < m_tPlayers.Length; ++i)
                    {
                        m_tGoals[i].SetActive(true);
                        m_tPlayers[i].GetComponent<PlayerControl>().Respawn();
                        m_tBall.GetComponent<BallControl>().Restart();
                    }
                    m_tTerrain.GetComponent<TerrainControl>().bChangeTerrain = true;

                    TimerBeforeStart = MaxTimer;
                }
            }
        }
    }

    void UpdateStartCountdown()
    {
        pUiStartCount.text = "" + (int)(TimerBeforeStart);
        if (TimerBeforeStart <= 0.0f)
        {
            pUiStartCount.text = "";
        }
    }
}
