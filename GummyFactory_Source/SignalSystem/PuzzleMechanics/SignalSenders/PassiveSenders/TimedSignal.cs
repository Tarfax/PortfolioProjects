using _Game.Scripts.Actors;
using System.Collections.Generic;
using UnityEngine;

public class TimedSignal : SignalSender {

    [Header("Settings")]
    [SerializeField] private float sendSignalIn = 1f;
    [SerializeField] private bool startActive = true;

    [Header("References")]
    [SerializeField] private List<SignalSender> sendersToInteract; 

    [Header("Debug")]
    [SerializeField] private bool isActive;
    [SerializeField] private bool reset;
    [SerializeField] private float timer;

    private void OnEnable() {
        DoReset();
    }

    private void Start() {
    }

    private void Update() {
        if (isActive == true) {
            timer -= Time.deltaTime;
            if (timer <= 0f) {
                foreach (var item in sendersToInteract) {
                    item.Interact(null);
                }
                SendSignal(true);
                isActive = false;
            }
        }
    }

    protected override void OnInteract(Actor actor) {
        DoReset();
    }

    public void DoReset() {
        timer = sendSignalIn;
        isActive = startActive;
    }

    private void OnValidate() {
        if (reset == true) {
            reset = false;
            DoReset();
        }
    }
}
