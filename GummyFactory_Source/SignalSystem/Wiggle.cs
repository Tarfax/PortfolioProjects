using UnityEngine;

public class Wiggle : MonoBehaviour {

    float startTime;
    public float animDur = 1.0f;

    Vector3 startPos, endPos;
    public float posRange = 2.0f;
    public bool noZMove = true;

    Vector3 startRot, endRot;
    public float rotRange = 1.0f;
    public bool lockRotToZ = false;

    private bool isDestroyed;
    private bool update = true;

    void Awake() {
        startPos = transform.localPosition;
        startRot = transform.eulerAngles;
        isDestroyed = false;
        MakeNewEnds();
    }

    float RandomRange(float a, float b) {
        if (a > b) {
            return Random.Range(b, a);
        }
        else {
            return Random.Range(a, b);
        }
    }

    void MakeNewEnds() {
        startTime = Time.time;

        endPos = new Vector3(
            RandomRange(startPos.x - posRange, startPos.x + posRange),
            RandomRange(startPos.y - posRange, startPos.y + posRange),
            noZMove ? 0 : RandomRange(startPos.z - posRange, startPos.z + posRange)
        );

        endRot = new Vector3(
            lockRotToZ ? 0 : RandomRange(startRot.x - rotRange, startRot.x + rotRange),
            lockRotToZ ? 0 : RandomRange(startRot.y - rotRange, startRot.y + rotRange),
            RandomRange(startRot.z - rotRange, startRot.z + rotRange)
        );
    }

    void Update() {
        if (update == true) {

            float timePassed = Time.time - startTime;
            float pctComplete = timePassed / animDur;

            if (pctComplete >= 1.0f) {
                MakeNewEnds();
                return;
            }

            if (pctComplete < 0.5f) {
                transform.localPosition = Vector3.Lerp(startPos, endPos, pctComplete * 2f);
                transform.eulerAngles = Vector3.Lerp(startRot, endRot, pctComplete * 2f);
            }
            else {
                // tween back
                float pct2 = (1.0f - pctComplete) * 2f;
                transform.localPosition = Vector3.Lerp(startPos, endPos, pct2);
                transform.eulerAngles = Vector3.Lerp(startRot, endRot, pct2);
            }
        }
    }

    public void Destroy(float time) {
        if (isDestroyed == false) {
            Destroy(gameObject.transform.parent.gameObject, time);
            isDestroyed = true;
        }
    }

    public void TurnOff() {
        update = false;
    }

    public void TurnOn() {
        update = true;
    }

}
