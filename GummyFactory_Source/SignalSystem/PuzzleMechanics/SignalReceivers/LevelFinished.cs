using MC_Utility.EventSystem;
using UnityEngine;
using UnityEngine.SceneManagement;

public class LevelFinished : SignalReceiver {

    private bool levelFinished;

    protected override bool OnReceiveSignal(bool newSignal) {
        if (levelFinished == false && newSignal == true) {
            levelFinished = true;
            EventSystem<LevelFinishedEvent>.FireEvent(LevelFinishedEvent.GetEvent());
            return true;
        }
        else if (levelFinished == true) {
            return true;
        }
        return false;
    }

}



public class LevelFinishedEvent : IEvent {

    public static LevelFinishedEvent GetEvent() {
        return new LevelFinishedEvent();
    }

}

