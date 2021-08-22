using System.Collections;
using System.Collections.Generic;
using System.IO;
using _Game.Scripts.Actors;
using UnityEngine;
using Pathfinding;
using Path = Pathfinding.Path;

public class AstarAI : ActorComponentBase
{
    public Transform TargetPosition;
    public Path Path;
    public float NextWayPointDistance = 3;
    public bool ReachedEndOfPath;
    
    private int currentWaypoint = 0;
    private Seeker seeker;

    
    void Start()
    {
        seeker = GetComponent<Seeker>();
        seeker.StartPath(transform.position, TargetPosition.position, OnPathCompleted);
    }

    public void OnPathCompleted(Path p)
    {
        if (p.error == false)
        {
            Path = p;
            currentWaypoint = 0;
        }   
    }

    
    void Update()
    {
        seeker.StartPath(transform.position, TargetPosition.position, OnPathCompleted);
        if (Path == null)
        {
            return;
        }

        ReachedEndOfPath = false;

        float distanceToWaypoint;
        while (true)
        {
            distanceToWaypoint = Vector3.Distance(transform.position, Path.vectorPath[currentWaypoint]);
            if (distanceToWaypoint < NextWayPointDistance)
            {
                if (currentWaypoint + 1 < Path.vectorPath.Count)
                {
                    currentWaypoint++;
                }
                else
                {
                    ReachedEndOfPath = true;
                    break;
                }
            }
            else
            {
                break;
            }
        }

        var speedFactor = ReachedEndOfPath ? Mathf.Sqrt(distanceToWaypoint / NextWayPointDistance) : 1f;

        Vector3 dir = (Path.vectorPath[currentWaypoint] - transform.position).normalized;
        
        Parent.SetSpeedFactor(speedFactor);
        Parent.SetMovementDirection(new Vector2(dir.x,dir.z));
        
        //controller.SimpleMove(velocity);
    }
    
    
}
