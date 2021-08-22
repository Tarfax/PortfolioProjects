
using System.Collections.Generic;
using MC_Utility;
using UnityEngine;
using UnityEngine.InputSystem;

public class ScoreManager : MonoBehaviour
{
    
    [Header("Time")]
    public int scorePerTimeInterval;
    public float timeInterval;
    [Header("Platform")]
    [Min(1)] public int scorePerSurvivedPlatform;
    [Min(1)] public int platformScoreCounterIncrement;
    [Range(0f, 1f)] public float platformScoreCounterSpeed;
    [Header("Projectile")] 
    public int projectileScore;
    [Header("Enemy")]
    
    // Timer
    private float timeIntervalTimer;
    public EnemyKillScore[] enemyScore;
    // 
    private static int platformScoreTotal;
    private int platformScoreMax;
    private float platformScoreCounterTimer;
    private static bool platformCounterActive;
    private int platformCount;
    // Player score
    private static int playerScore;
    private static string playerScoreFormatted;
    // Reached max score
    private static bool reachedMaxScore;
    // Player name
    private static string playerName;
    // High score list
    private static List<string> highScoreNames = new List<string>();
    private static List<string> highScorePointsString = new List<string>();
    private List<int> highScorePointsInt = new List<int>();
    // High score keys
    private int highScoreCount = 5;
    private string highScoreNameKey = "HighScoreName";
    private string highScorePointStringKey = "HighScorePointString";
    private string highScorePointIntKey = "HighScorePointInt";
    // New high score
    private static bool newHighScore;
    

    //***************************************************************************************************************
    
    private void Start()
    {
        // Player score
        playerScore = 0;
        SetPlayerScoreString();
        reachedMaxScore = false;
        // Timer
        timeIntervalTimer = timeInterval;

        platformScoreTotal = 0;
        platformScoreMax = 0;
        platformScoreCounterTimer = 0;
        platformCounterActive = false;
        platformCount = 0;

        newHighScore = false;
        
        // High score
        CreateHighScore();
        GetAllHighScores();
    }


    private void Update()
    { 
        // Time score
        TimeIntervalTimer();
        // Platform counter
        PlatformCounter();
        
        // Reset all high scores
        if (Keyboard.current.f12Key.wasPressedThisFrame == true) {
            ResetHighScores();
        } 
    }

    //***************************************************************************************************************
    
    // Unregister Event
    private void OnDisable()
    {
        EventSystem<EnemyDeathEvent>.UnregisterListener(OnDeathEvent);
        EventSystem<PlayerAbsorbedProjectileEvent>.UnregisterListener(OnProjectileHitEvent);
        EventSystem<GameOverEvent>.UnregisterListener(OnGameOver);
    }

    // Register Event
    private void OnEnable()
    {
        EventSystem<EnemyDeathEvent>.RegisterListener(OnDeathEvent);
        EventSystem<PlayerAbsorbedProjectileEvent>.RegisterListener(OnProjectileHitEvent);
        EventSystem<GameOverEvent>.RegisterListener(OnGameOver);
    }
    
    
    // Enemy death event
    private void OnDeathEvent(EnemyDeathEvent deathEvent)
    {
        
        EnemyType enemy = deathEvent.EnemyType;

        int loops = enemyScore.Length; 
        for (var i = 0; i < loops; i++)
        {
            if (enemy == enemyScore[i].enemy)
            {
                // Score
                AddPlayerScore(enemyScore[i].killScore);
            }
        }
        
    }
    
    // Player absorbed projectile event
    private void OnProjectileHitEvent(PlayerAbsorbedProjectileEvent playerAbsorbedProjectileEvent)
    {
        AddPlayerScore(projectileScore);
    }
    
    // Game over event
    private void OnGameOver(GameOverEvent gameOver)
    {
        CheckForNewHighScore();
    }
    
    
    //***************************************************************************************************************


    // Platform counter
    private void PlatformCounter()
    {
        if (EnemySpawnerManager.BossCleared() == true)
        {
            platformScoreTotal = 0;
            platformCount = PlatformHealthPoint.Count;
            platformScoreMax = scorePerSurvivedPlatform * platformCount;
            platformScoreCounterTimer = platformScoreCounterSpeed;
            platformCounterActive = true;
        }
        
        if (platformCounterActive == false)
        {
            return;
        }
        
        
        bool done = ScoreCounter(ref platformScoreTotal, platformScoreMax, platformScoreCounterIncrement, 
            ref platformScoreCounterTimer, platformScoreCounterSpeed);
        if (done == true)
        {
            platformCounterActive = false;
            AddPlayerScore(platformScoreTotal);
        }
    }

    // Get platform score string
    public static string GetPlatformScoreString()
    {
        return platformScoreTotal.ToString();
    }

    // Platform counter active
    public static bool GetPlatformCounterActive()
    {
        return platformCounterActive;
    }
    
    
    // Score Counter
    private bool ScoreCounter(ref int totalScore, int maxScore, int addScore, ref float timer, float timerReset)
    {
        // Countdown
        timer -= Time.deltaTime;
        if (timer <= 0f)
        {
            totalScore += addScore;
            timer = timerReset;

            if (totalScore >= maxScore)
            {
                totalScore = maxScore;
                
                return true;
            }
        }
        
        return false;
    }

    // Add player score
    private void AddPlayerScore(int score)
    {
        playerScore += score;
        if (playerScore >= 999999)
        {
            playerScore = Mathf.Clamp(playerScore, 0, 999999);
            reachedMaxScore = true;
        }
        
        // Add to string
        SetPlayerScoreString();
    }
    
    // Set player score string
    private void SetPlayerScoreString()
    {
        string scoreString = playerScore.ToString();
        int stringSize = scoreString.Length;
        string zero = "000000";
        string final = zero.Remove(0, stringSize) + scoreString;
        
        playerScoreFormatted = final;
    }
    
    // Get player score string
    public static string GetPlayerScoreString()
    {
        return playerScoreFormatted;
    }
    
    // Max score reached
    public static bool ReachedMaxScore()
    {
        return reachedMaxScore;
    }
    
    // Set time interval timer
    private void SetTimeIntervalTimer()
    {
        timeIntervalTimer = timeInterval;
    }
    
    // Time interval Timer
    private void TimeIntervalTimer()
    {
        timeIntervalTimer -= Time.deltaTime;
        if (timeIntervalTimer <= 0f)
        {
            AddPlayerScore(scorePerTimeInterval);
            SetTimeIntervalTimer();
        }
    }
    
    // Set player name
    public static void SetPlayerName(string playerName)
    {
        ScoreManager.playerName = playerName;
    }
    
    // Get player name
    public static string GetPlayerName()
    {
        return playerName;
    }

    
    //***************************************************************************************************************
    

    // New high score
    public static bool NewHighScore()
    {
        return newHighScore;
    }
    
    
    // Check for new high score
    private void CheckForNewHighScore()
    {
        newHighScore = false;
        
        for (var i = 0; i < highScoreCount; i++)
        {
            int highScore = highScorePointsInt[i];

            if (playerScore > highScore)
            {
                highScoreNames[i] = playerName;
                highScorePointsString[i] = playerScoreFormatted;
                highScorePointsInt[i] = playerScore;
                
                PlayerPrefs.SetString(highScoreNameKey+i.ToString(), playerName);
                PlayerPrefs.SetString(highScorePointStringKey+i.ToString(), playerScoreFormatted);
                PlayerPrefs.SetInt(highScorePointIntKey+i.ToString(), playerScore);
                PlayerPrefs.Save();
                
                newHighScore = true;
            }
        }
    }

    
    // Create high score
    private void CreateHighScore()
    {
        bool highScoreExist = PlayerPrefs.HasKey("HighScore");
        
        // Create
        if (highScoreExist == false)
        {
            PlayerPrefs.SetString("HighScore", "Created");

            for (var i = 0; i < highScoreCount; i++)
            {
                int keyIndex = i;
                PlayerPrefs.SetString(highScoreNameKey+keyIndex.ToString(), "-");
                PlayerPrefs.SetString(highScorePointStringKey+keyIndex.ToString(), "-");
                PlayerPrefs.SetInt(highScorePointIntKey+keyIndex.ToString(), 0);
                PlayerPrefs.Save();
            }
        }
    }
    
    
    // Get all high scores
    private void GetAllHighScores()
    {
        for (var i = 0; i < highScoreCount; i++)
        {
            int keyIndex = i ;
            string name = PlayerPrefs.GetString(highScoreNameKey + keyIndex.ToString());
            string pointString = PlayerPrefs.GetString(highScorePointStringKey + keyIndex.ToString());
            int pointInt = PlayerPrefs.GetInt(highScorePointIntKey + keyIndex.ToString());
            
            highScoreNames.Add(name);
            highScorePointsString.Add(pointString);
            highScorePointsInt.Add(pointInt);
        }
    }
    
    
    // Reset high scores
    public void ResetHighScores()
    {
        for (var i = 0; i < highScoreCount; i++)
        {
            int keyIndex = i;
            PlayerPrefs.SetString(highScoreNameKey+keyIndex.ToString(), "-");
            PlayerPrefs.SetString(highScorePointStringKey+keyIndex.ToString(), "-");
            PlayerPrefs.SetInt(highScorePointIntKey+keyIndex.ToString(), 0);
            PlayerPrefs.Save();
        }
    }

    
    // Get high score name
    public static string GetHighScoreName(int position)
    {
        return highScoreNames[position];
    }
    
    // Get high score point
    public static string GetHighScorePoint(int position)
    {
        return highScorePointsString[position];
    }
    
}
