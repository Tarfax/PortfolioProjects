using UnityEngine;

public class ObjectRotator : SignalReceiver {

    [Header("Settings")]
    [Tooltip("Should be a child object")]
    [SerializeField] private Transform transformToRotate;
    [Space]
    [SerializeField] private float startRotation = 0f;
    [SerializeField] private float endRotation = 110f;
    [SerializeField] private int rotationsToMake = 4;
    [SerializeField] private float rotationDuration = 1f;

    [Header("Debug")]
    [SerializeField] private int rotationIntervall;
    [SerializeField] private bool isActive = false;
    [SerializeField] private bool isFinished = false;
    [SerializeField] private bool reset;
    private float time;
    private bool doRotate;
    private float currentEndRotation;
    private Vector3 currentStartRotation;

    private void Start() {
        Reset();
    }

    void Update() {
        if (reset == true) {
            Reset();
        }

        if (doRotate == true && isActive == true) {
            time += Time.deltaTime;
            float t = time / rotationDuration;
            t = t * t * (3f - 2f * t);

            Vector3 eulerRotation = Vector3.Lerp(currentStartRotation, new Vector3(0f, currentEndRotation, 0f), t);
            transformToRotate.rotation = Quaternion.Euler(eulerRotation);

            if (time >= rotationDuration) {
                doRotate = false;
                time = 0f;
                isActive = false;
            }
        }
    }

    private void Reset() {
        reset = false;
        isFinished = false;
        transformToRotate.rotation = Quaternion.Euler(Vector3.zero);
        rotationIntervall = 0;
        doRotate = false;

        isActive = true;
        //ReceiveSignal(false);
        isActive = false;
    }

    protected override bool OnReceiveSignal(bool newSignal) {
        if (isFinished == false && isActive == false) {
            doRotate = true;
            rotationIntervall++;
            isActive = true;

            currentStartRotation = transformToRotate.localRotation.eulerAngles;
            currentEndRotation = Mathf.Lerp(startRotation, endRotation, rotationIntervall / (float)rotationsToMake);
        }

        if (rotationIntervall >= rotationsToMake) {
            isFinished = true;
            return true;
        }

        return false;
    }

}
