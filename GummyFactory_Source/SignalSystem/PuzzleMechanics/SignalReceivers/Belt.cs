using UnityEngine;

[ExecuteInEditMode]
public class Belt : SignalReceiver {

    private Transform startPosition;
    private Transform midPosition;
    private Transform endPosition;

    [Header("Belt Settings")]
    [Range(0.1f, 3f)] [SerializeField] private float speed = 1;
    [SerializeField] private bool isCorner;
    [SerializeField] private bool forwardDirection;
    [SerializeField] private bool isGreenSignal;
    private GameObject objectToMove;
    private bool startMoving;
    private bool hasObject;

    [Header("References")]
    [SerializeField] private GameObject pointA;
    [SerializeField] private GameObject pointB;
    [SerializeField] private GameObject pointC;
    [SerializeField] private GameObject meshA;
    [SerializeField] private GameObject meshB;

    [SerializeField] private Belt connectedBeltPointA;
    [SerializeField] private Belt connectedBeltPointB;
    private Belt nextBelt;

    [SerializeField] private GameObject beltDirection;


    [Header("Debug")]
    [SerializeField] private bool hasReachedDestination;
    [SerializeField] private float timeStep;


    void Start() {
        SetDirection(true);
        midPosition = pointB.transform;
        timeStep = 0f;
        hasObject = false;
    }

    void Update() {
        if (startMoving == true) {
            MoveItem();
            MoveToNextBelt();
        }


        //if (Input.GetKeyDown(KeyCode.C) == true) {
        //    SetSignal(!isGreenSignal);
        //}

    }

    private void OnValidate() {
        if (isCorner == false) {
            meshB.SetActive(false);
        }
        else {
            meshB.SetActive(true);
        }

        ChangeDirection(forwardDirection);
    }

    private void MoveToNextBelt() {
        if (hasReachedDestination == true && hasObject == true) {
            if (nextBelt != null) {
                if (nextBelt.hasObject == false) {
                    nextBelt.AttachObject(objectToMove);
                }
                hasObject = false;
                objectToMove = null;
                hasReachedDestination = false;
                startMoving = false;
            }
        }
    }

    private void MoveItem() {
        if (timeStep < 1.0f && hasObject) {
            if (isGreenSignal == true) {
                timeStep += speed * Time.deltaTime;

                if (isCorner == true) {
                    Vector3 m1 = Vector3.Lerp(startPosition.position, midPosition.position, timeStep);
                    Vector3 m2 = Vector3.Lerp(midPosition.position, endPosition.position, timeStep);

                    objectToMove.transform.position = Vector3.Lerp(m1, m2, timeStep);
                    Vector3 v3 = m2 - m1;
                    v3.y = 0f;
                    objectToMove.transform.rotation = Quaternion.LookRotation(v3);
                }
                else {
                    objectToMove.transform.position = Vector3.Lerp(startPosition.position, endPosition.position, timeStep);
                }

                hasReachedDestination = false;

            }
        }
        else {
            hasReachedDestination = true;
        }
    }

    public void AttachObject(GameObject objectToMove) {
        this.objectToMove = objectToMove;
        startMoving = true;
        hasObject = true;
        objectToMove.transform.SetParent(transform);
        objectToMove.transform.localPosition = startPosition.localPosition;
        objectToMove.transform.rotation = startPosition.rotation;
    }

    private void SetDirection(bool forwardDirection) {
        if (hasObject == true) {
            timeStep = Mathf.Abs(timeStep - 1);
        }
        else {
            timeStep = 0f;
        }

        GameObject endPoint = isCorner ? pointC : pointB;

        if (forwardDirection == true) {
            endPosition = endPoint.transform;
            startPosition = pointA.transform;
            nextBelt = connectedBeltPointB;
        }
        else {
            endPosition = pointA.transform;
            nextBelt = connectedBeltPointA;
            startPosition = endPoint.transform;
        }

        if (objectToMove != null) {
            objectToMove.transform.rotation = startPosition.rotation;
        }

        beltDirection.transform.rotation = Quaternion.LookRotation(endPosition.position - startPosition.position, Vector3.up);

    }

    private void ChangeDirection(bool direction) {
        forwardDirection = direction;
        SetDirection(direction);

        if (connectedBeltPointA != null && connectedBeltPointA.forwardDirection != direction) {
            connectedBeltPointA.ChangeDirection(direction);
        }
        if (connectedBeltPointB != null && connectedBeltPointB.forwardDirection != direction) {
            connectedBeltPointB.ChangeDirection(direction);
        }
    }

    protected override bool OnReceiveSignal(bool isGreen) {
        isGreenSignal = isGreen;
        return isGreenSignal;
    }

}
