
using System;
using System.Collections.Generic;
using System.Linq;
using MC_Utility;
using UnityEngine;
using Random = UnityEngine.Random;

public class EnemySpawner : MonoBehaviour
{

    [Header("Player")] public Transform player;

    [Header("Spawer Duration")] 
    [Range(1, 3)] public int durationType = 1;
    public bool spawnStage;
    public bool clearStage;
    public bool timeLimit;
    [Min(0)] public float spawnerDuration;
    
    [Header("Spawn point settings")]                           [Tooltip("Number of spawn rows.")]
    [Range(1, 10)]public int spawnRows = 1;                    [Tooltip("Number of spawn points per row.")] 
    [Range(1, 10)] public int spawnPointsPerRow = 1;           [Tooltip("Space between spawn points in row.")]
    [Range(0.01f, 1f)] public float spawnPointSpacing = 1f;    [Tooltip("Radius size for spawn collision.")] 
    [Range(0f, 1f)] public float spawnPointRadiusCheck = 0.2f; [Tooltip("Spawn collision layer to check against.")]      
    public LayerMask spawnPointLayerCheck;                     
    
    
    [Header("Global intensity settings")] [Space(15)]
    public GlobalIntensitySetup[] GlobalIntensity;
    [Min(0f)] private float[] globalIntensityTimePoint;
    [Min(0f)] private float[] globalIntensity;         
    
    
    [Header("Spawn Settings")] [Space(15)]          
    public SpawnSetup[] Spawn;
    private GameObject[] enemies;
    private bool[] spawnTop;
    private bool[] spawnLimited;
    private int[] spawnLimit;
    private int[] spawnActiveMax;
    [Min(0f)] private float[] startDelayMin;
    [Min(0f)] private float[] startDelayMax;
    [Min(0f)] private float[] spawnRateMin;
    [Min(0f)] private float[] spawnRateMax;
    [Range(1f, 10f)] private float[] spawnRateIntensity;
    [Range(0f, 1f)] private float[] spawnProbability;
    [Range(0f, 1f)] private float[] minimumAvoidance;
    
    
    // Array size
    private int arraySize;
    // Stage
    private bool nextStage;
    // Spawn count
    private int[] spawnLimitCount;
    private int spawnLimitCountMax;
    private int spawnLimitCountDone;
    // Active count
    private int[] spawnActiveMaxIndex;
    private List<List<int>> spawnActiveMaxListHolder = new List<List<int>>();
    // Clear count
    private int[] spawnClearCount;
    private int spawnClearCountMax;
    private int spawnClearCountDone;
    private bool[] spawnClearDone;
    private int[] spawnClearIndex;
    private List<List<int>> spawnClearListHolder = new List<List<int>>();
    // Timer & timer states
    private int spawnerDurationTimer;
    //
    private float[] startDelayTimer;
    private bool[] startDelayTimerDone;
    private int startDelayTimerDoneCounter;
    //
    private float[] spawnRateTimer;
    // Global intensity
    private float activeSpawnerTime;
    private float globalIntensityTimer;
    private int globalIntensityIndex;
    private int globalIntensityMaxIndex;
    private float globalIntensityLocal;
    private bool canIntensify;
    // Positions
    private Vector3[] spawnPositions;
    private Vector3[] spawnPositionsTop;
    private Vector3[] spawnPositionsHolder;
    private bool spawnPositionAvailable;
    // Spawn queue
    private List<int> spawnQueue = new List<int>();
    // Enemy spawn manager
    private EnemySpawnerManager enemySpawnerManager;
    // 
    private GameObject lastSpawnedEnemy;
    
    
    
    //************************************************************************************************************** 

    private void Start()
    {
        // Enemy spawn manager
        enemySpawnerManager = GetComponentInParent<EnemySpawnerManager>();
        // Positions
        CreateSpawnPositions();
        // Start up
        SpawnerStartup();
        // Deactivate self
        //gameObject.SetActive(false);
    }

    private void Update()
    {
        // Active time
        activeSpawnerTime += Time.deltaTime;

        // Time limit
        if(timeLimit == true){
            // Next stage
            if (activeSpawnerTime >= spawnerDuration)
            {
                nextStage = true;
                //enemySpawnerManager.NextSpawnStage();
            }
        }
        
        // Spawn from queue
        if (spawnPositionAvailable == false)
        {
            SpawnFromQueue();

            // No more positions available
            if (spawnPositionAvailable == false)
            {
                return;
            }
        }
        
        // Start delay
        StartDelayTimer();
        // Spawn
        SpawnRateTimer();
        // Global intensity 
        GlobalIntensityTimer();
        
        // Next stage
        if (nextStage)
        {
            if (spawnPositionAvailable == false)
            {
                return;
            }
            enemySpawnerManager.NextSpawnStage();
        }
    }
    
    //**************************************************************************************************************   
    
    private void OnValidate()
    {

        switch (durationType)
        {
            case 1:
                spawnStage = true;
                clearStage = false;
                timeLimit = false;
                break;
            case 2:
                spawnStage = false;
                clearStage = true;
                timeLimit = false;
                break;
            case 3:
                spawnStage = false;
                clearStage = false;
                timeLimit = true;
                break;
        }
        
        if (timeLimit == false)
        {
            spawnerDuration = 0f;
        }

        int loops = Spawn.Length;
        for (var i = 0; i < loops; i++)
        {
            if (Spawn[i].spawnLimited == false)
            {
                Spawn[i].spawnLimit = 0;
            }
        }
        
    }
    
    private void OnDrawGizmos()
    {
        CreateSpawnPositions();
        int loops = spawnPositions.Length;
        for (var i = 0; i < loops; i++)
        {

            Gizmos.color = Color.red;
            Gizmos.DrawSphere(spawnPositions[i], spawnPointRadiusCheck);

            if (i < spawnPointsPerRow)
            {
                Gizmos.color = Color.yellow;
                Gizmos.DrawSphere(spawnPositionsTop[i], spawnPointRadiusCheck);
            }
        }/**/
    }/**/

    //**************************************************************************************************************
    
    // Next stage
    private void NextStage()
    {
        enemySpawnerManager.NextSpawnStage();
    }
    
    // Unregister Event
    private void OnDisable()
    {
        //spawnActiveMaxListHolder.Clear();
        //spawnClearListHolder.Clear();
        //spawnQueue.Clear();
        EventSystem<EnemyDeathEvent>.UnregisterListener(OnDeathEvent);
    }

    // Register Event
    private void OnEnable()
    {
        EventSystem<EnemyDeathEvent>.RegisterListener(OnDeathEvent);
    }

    private void OnDeathEvent(EnemyDeathEvent deathEvent)
    {
        
        bool foundID = false;
        int instanceID = deathEvent.GameObject.GetInstanceID();
        
        int loopsTier1 = arraySize;
        for (var i=0; i < loopsTier1; i++)
        {
            // Only check limited (list)
            if (spawnActiveMax[i] == 0)
            {
                continue;
            }
            
            List<int> instanceIDList = spawnActiveMaxListHolder[spawnActiveMaxIndex[i]];
            
            int loopsTier2 = spawnActiveMaxListHolder[spawnActiveMaxIndex[i]].Count;
            for (var j = 0; j < loopsTier2; j++)
            {
                
                // Match
                if (instanceIDList[j] == instanceID)
                {
                    instanceIDList.RemoveAt(j);

                    // Stop loop
                    foundID = true;
                    break;
                }
            }
            
            // Stop loop
            if (foundID == true)
            {
                break;
            }
        }
        
        
        
        // Clear stage
        if (clearStage == false)
        {
            return;
        }
        
        foundID = false;
        instanceID = deathEvent.GameObject.GetInstanceID();
        
        loopsTier1 = arraySize;
        for (var i=0; i < loopsTier1; i++)
        {
            // Only check limited (list)
            if (spawnLimited[i] == false || spawnClearDone[i] == true)
            {
                continue;
            }
            
            List<int> instanceIDList = spawnClearListHolder[spawnClearIndex[i]];
            
            int loopsTier2 = spawnClearListHolder[spawnClearIndex[i]].Count;
            for (var j = 0; j < loopsTier2; j++)
            {
                
                // Match
                if (instanceIDList[j] == instanceID)
                {
                    instanceIDList.RemoveAt(j);
                    spawnClearCount[i]++;

                    // List done
                    if (spawnClearCount[i] == spawnLimit[i])
                    {
                        spawnClearCountDone++;
                        spawnClearDone[i] = true;
                    }
                    
                    // Stop loop
                    foundID = true;
                    break;
                }
            }
            
            // Stop loop
            if (foundID == true)
            {
                break;
            }
        }

        // Next stage
        if (spawnClearCountDone == spawnClearCountMax)
        {
            nextStage = true;
        }
    }
    
    // GetSpawnActiveMax
    public int GetSpawnActiveMax(int enemyType)
    {

        return 0;
    }
    
    
    // Spawn from queue
    private void SpawnFromQueue()
    {
        spawnPositionAvailable = true;
        
        int loops = spawnQueue.Count;
        for (var i = 0; i < loops; i++)
        {
            int enemyType = spawnQueue[i];

            // Position
            Vector3 position = GetSpawnPosition(enemyType);
            
            // No more positions available
            if (spawnPositionAvailable == false)
            {
                return;
            }
            
            // Spawn max active
            if (spawnActiveMax[i] > 0)
            {
                if (spawnActiveMaxListHolder[spawnActiveMaxIndex[enemyType]].Count == spawnActiveMax[enemyType])
                {
                    continue;
                }
            }
            
            // Remove index
            spawnQueue.RemoveAt(i);
            // Instantiate
            lastSpawnedEnemy = Instantiate(enemies[enemyType], position, Quaternion.identity);
            SpawnLimitCounter(enemyType);
            AddToSpawnActiveList(enemyType);
        }
    }
    
    // Set spawn positions
    private void CreateSpawnPositions()
    {

        List<AI_Path> list = AI_Grid.GetPathList();
        float width = Vector3.Distance(list[0].StartPosition, list[0].EndPosition);
        float marginXAxis = (width/(spawnPointsPerRow-0.99f)) * spawnPointSpacing;
        float marginYAxis = Vector3.Distance(list[0].StartPosition, list[1].EndPosition);
        int rows = list.Count() - 1;
        spawnRows = Mathf.Clamp(spawnRows, 1, rows);
        rows = spawnRows;
        spawnPositions = new Vector3[rows*spawnPointsPerRow];
        spawnPositionsTop = new Vector3[spawnPointsPerRow];
        float xPosition = list[0].StartPosition.x;
        float yPosition = list[0].StartPosition.y;
        int directionToggle = 1;
        int counter = 0;
        
        // Set positions
        int loops = rows;
        for (var i = 0; i < loops; i++)
        {
            float yPos = yPosition - (marginYAxis * i);
            
            // Swap
            if (i % 2 == 0) {
                xPosition = list[0].StartPosition.x;
                directionToggle = 1;
            }
            else {
                xPosition = list[0].EndPosition.x;
                directionToggle = -1;
            }
            
            // Colums
            for (var j = 0; j < spawnPointsPerRow; j++)
            {
                float xPos = xPosition + (marginXAxis * j) * directionToggle;
                spawnPositions[counter] = new Vector3(xPos, yPos, list[0].StartPosition.z);
                
                // Top (exception)
                if (i < 1)
                {
                    float xPosTop = xPosition + (width / (Mathf.Clamp(spawnPointsPerRow - 0.99f, 0.99f, 100))) * j;
                    if (spawnPointsPerRow == 1)
                    {
                        xPosTop = xPosition + (width / 2);
                    }
                    float yPosTop = yPosition + marginYAxis;
                    spawnPositionsTop[counter] = new Vector3(xPosTop, yPosTop, list[0].StartPosition.z);
                }

                counter++;
            }
        }

    }

    // Get Spawn position AVOIDANCE DISABLED
    private Vector3 GetSpawnPosition(int enemyType)
    {
        // Regular positions
        Vector3[] spawnPositionsHolder = spawnPositions;
        
        // Change to top positions
        if (spawnTop[enemyType] == true)
        {
            spawnPositionsHolder = spawnPositionsTop;
        }
        
        // Default position to stack on if none is free
        Vector3 position = spawnPositionsHolder[0];
        
        // Avoidance
        if (minimumAvoidance[enemyType] > 0)
        {
            
        }
        
        
        // Random
        //else
        //{
            // Create and populate list
            List<int> spawnPositionIndexes = new List<int>();
            int loops = spawnPositionsHolder.Length;
            for (var i = 0; i < loops; i++)
            {
                spawnPositionIndexes.Add(i);
            }
            
            bool positionFound = false;
            // Find random available position
            for (var i = 0; i < loops; i++)
            {
                int maxIndex = spawnPositionIndexes.Count();
                int randomIndex = Random.Range(0, maxIndex);
                int index = spawnPositionIndexes[randomIndex];
                spawnPositionIndexes.RemoveAt(randomIndex);
                position = spawnPositionsHolder[index];

                if (Physics.CheckSphere(position, spawnPointRadiusCheck, spawnPointLayerCheck) == true)
                {
                    continue;
                }

                positionFound = true;
                break;
            }
            // Clear list
            spawnPositionIndexes.Clear();

            spawnPositionAvailable = positionFound;
            
        //}
        
        return position;
    }
    
    
    // Global intensity timer
    private void GlobalIntensityTimer()
    {
        if (canIntensify == false)
        {
            return;
        }
        
        // Set new intensity
        globalIntensityTimer -= Time.deltaTime;
        if (globalIntensityTimer <= 0f)
        {
            // Set intensity 
            globalIntensityLocal = globalIntensity[globalIntensityIndex];
            
            // Next intensity change
            globalIntensityIndex++;
            
            // No more changes
            if (globalIntensityIndex < globalIntensityMaxIndex)
            {
                // Set next timer
                globalIntensityTimer = globalIntensityTimePoint[globalIntensityIndex];
                return;
            }

            globalIntensityIndex--;
            canIntensify = false;
        }
    }
    
    // Spawner startup
    private void SpawnerStartup()
    {
        // Set array size
        arraySize = Spawn.Length;
        // Spawn count
        spawnLimitCount = new int[arraySize];
        spawnLimitCountMax = 0;
        spawnLimitCountDone = 0;
        // Active count
        spawnActiveMaxIndex = new int[arraySize];
         // Clear count
        spawnClearCount = new int[arraySize];
        spawnClearCountMax = 0;
        spawnClearCountDone = 0;
        spawnClearDone = new bool[arraySize];
        spawnClearIndex = new int[arraySize];
        // Spawn setups
        enemies = new GameObject[arraySize];
        spawnTop = new bool[arraySize];
        spawnLimited = new bool[arraySize];
        spawnLimit = new int[arraySize];
        spawnActiveMax = new int[arraySize];
        startDelayMin = new float[arraySize];
        startDelayMax = new float[arraySize];
        spawnRateMin = new float[arraySize];
        spawnRateMax = new float[arraySize];
        spawnRateIntensity = new float[arraySize];
        spawnProbability = new float[arraySize];
        minimumAvoidance = new float[arraySize];
        // Next stage
        nextStage = false;
        // Timers & timer states
        startDelayTimer = new float[arraySize];
        startDelayTimerDone = new bool[arraySize];
        startDelayTimerDoneCounter = 0;
        //
        spawnRateTimer = new float[arraySize];
        // Positions
        spawnPositionAvailable = true;
        // Spawn queue
        spawnQueue.Clear();

        // Spawn setup
        int loops = arraySize;
        for (var i = 0; i < loops; i++)
        {
            enemies[i] = Spawn[i].enemy;
            spawnTop[i] = Spawn[i].spawnTop;
            spawnLimited[i] = Spawn[i].spawnLimited;
            spawnLimit[i] = Spawn[i].spawnLimit;
            spawnActiveMax[i] = Spawn[i].spawnActiveMax;
            startDelayMin[i] = Spawn[i].startDelayMin;
            startDelayMax[i] = Spawn[i].startDelayMax;
            spawnRateMin[i] = Spawn[i].spawnRateMin;
            spawnRateMax[i] = Spawn[i].spawnRateMax;
            spawnRateIntensity[i] = Spawn[i].spawnRateIntensity;
            spawnProbability[i] = Spawn[i].spawnProbability;
            minimumAvoidance[i] = 0;//Spawn[i].minimumAvoidance;
            
            startDelayTimer[i] = Random.Range(startDelayMin[i], startDelayMax[i]);
            startDelayTimerDone[i] = false;
            spawnRateTimer[i] = Random.Range(spawnRateMin[i], spawnRateMax[i]);
            
            // Spawn count
            spawnLimitCount[i] = 0;
            spawnClearCount[i] = 0;

            if (Spawn[i].spawnLimit > 0)
            {
                spawnLimitCountMax++;

                // Clear stage
                if (clearStage)
                {
                    spawnClearCountMax++;
                    List<int> clearList = new List<int>();
                    spawnClearListHolder.Add(clearList);
                    spawnClearIndex[i] = spawnClearListHolder.Count-1;
                }
            }

            if (Spawn[i].spawnActiveMax > 0)
            {
                List<int> activeList = new List<int>();
                spawnActiveMaxListHolder.Add(activeList);
                spawnActiveMaxIndex[i] = spawnActiveMaxListHolder.Count-1;
            }
        }
        
        
        // Global intensity
        globalIntensityIndex = 0;
        globalIntensityMaxIndex = GlobalIntensity.Length;
        globalIntensityTimePoint = new float[globalIntensityMaxIndex];
        globalIntensity = new float[globalIntensityMaxIndex];
        
        loops = globalIntensityMaxIndex;
        for (var i = 0; i < loops; i++)
        {
            globalIntensityTimePoint[i] = GlobalIntensity[i].globalIntensityTimePoint;
            globalIntensity[i] = GlobalIntensity[i].globalIntensity;
        }

        globalIntensityTimer = globalIntensityTimePoint[globalIntensityIndex];
        globalIntensityLocal = 1;
        canIntensify = true;
    }

    // Set spawn timer
    private void SetSpawnRateTimer(int enemyType)
    {
        spawnRateTimer[enemyType] = Random.Range(spawnRateMin[enemyType], spawnRateMax[enemyType]) + spawnRateTimer[enemyType];
        
    }
    
    // Spawn timer
    private void SpawnRateTimer()
    {
        int loops = arraySize;
        for (var i = 0; i < loops; i++)
        {
            // Start delay check
            if (startDelayTimerDone[i] == false)
            {
                continue;
            }
            // Spawn limit Check
            if (spawnLimited[i] == true)
            {
                if (spawnLimitCount[i] == spawnLimit[i])
                {
                    continue;
                }
            }
            // Spawn max active
            if (spawnActiveMax[i] > 0)
            {
                if (spawnActiveMaxListHolder[spawnActiveMaxIndex[i]].Count == spawnActiveMax[i])
                {
                    continue;
                }
            }
            

            // Intensity
            float spawnInstensity = 1f;
            if (globalIntensityLocal != 1f)
            {
                spawnInstensity = spawnRateIntensity[i];
            }
            // Spawn timer countdown
            spawnRateTimer[i] -= spawnInstensity * globalIntensityLocal * Time.deltaTime;
            if (spawnRateTimer[i] > 0f)
            {
                continue;
            }
            
            // Set timer
            SetSpawnRateTimer(i);
            // Probability
            if (Probability(i) == true)
            {
                // No more positions available
                if (spawnPositionAvailable == false) 
                {
                    spawnQueue.Add(i);
                    continue;
                }
            }
            
            // Position
            Vector3 position = GetSpawnPosition(i);

            // No more positions available
            if (spawnPositionAvailable == false) 
            {
                spawnQueue.Add(i);
                continue;
            }

            lastSpawnedEnemy = Instantiate(enemies[i], position, Quaternion.identity);
            SpawnLimitCounter(i);
            AddToSpawnActiveList(i);
        }
    }

    // Probability
    private bool Probability(int enemyType)
    {
        float randomChance = Random.Range(0f, 1f);
        return randomChance <= spawnProbability[enemyType];
    }
    
    // Add to spawn active list
    private void AddToSpawnActiveList(int enemyType)
    {
        if (spawnActiveMax[enemyType] == 0)
        {
            return;
        }
        spawnActiveMaxListHolder[spawnActiveMaxIndex[enemyType]].Add(lastSpawnedEnemy.GetInstanceID());
    }

    // Add to spawn clear list
    private void AddToSpawnClearList(int enemyType)
    {
        if (clearStage == false || spawnLimited[enemyType] == false)
        {
            return;
        }

        spawnClearListHolder[spawnClearIndex[enemyType]].Add(lastSpawnedEnemy.GetInstanceID());
    }

    // Spawn limit counter
    private void SpawnLimitCounter(int enemyType)
    {
        if (spawnLimitCountDone == arraySize)
        {
            return;
        }
        
        // Add to clear list
        AddToSpawnClearList(enemyType);
        
        spawnLimitCount[enemyType]++;
        if (spawnLimitCount[enemyType] == spawnLimit[enemyType])
        {
            // Add to clear list
            //AddToSpawnClearList(enemyType);
            
            spawnLimitCountDone++;
            if (spawnLimitCountDone != spawnLimitCountMax)
            {
                return;
            }
            
            // Next stage
            if (spawnStage)
            {
                nextStage = true;
            }
        }
        
    }
    
    // Start delay timer
    private void StartDelayTimer()
    {
        if (startDelayTimerDoneCounter == arraySize)
        {
            return;
        }
        
        int loops = arraySize;
        for (var i = 0; i < loops; i++)
        {
            // Start timer countdown
            startDelayTimer[i] -= Time.deltaTime;
            if (startDelayTimer[i] > 0f || startDelayTimerDone[i] == true)
            {
                continue;
            }


            // Start delay done
            startDelayTimerDone[i] = true;
            startDelayTimerDoneCounter++;
        }
    }
}
