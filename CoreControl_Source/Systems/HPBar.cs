using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HPBar : MonoBehaviour
{

    private readonly int StartHP = 12;
    private int currentHP = 12;

    [SerializeField] private HPSocket[] sockets;

    private int currentSocket = 0;


    public void Damage()
    {
        currentHP--;
        
        if (currentHP % 2 == 1)
        {
            //reduce brightness
            if (currentSocket < sockets.Length)
                sockets[currentSocket].Half();
        } 
        else
        {
            //turn off 
            if (currentSocket < sockets.Length)
                sockets[currentSocket].Off();
            //next socket
            currentSocket++;
        }
        
    }

    public bool Alive()
    {
        return currentHP >= 1;
    }

}
