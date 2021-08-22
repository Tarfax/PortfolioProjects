using System;
using UnityEngine;

[Serializable]
public struct AI_Path {
    public Vector3 StartPosition;
    public Vector3 EndPosition;
    public int Row;

    private bool isLeftDirection;

    public AI_Path(Vector3 startPosition, Vector3 endPosition, bool isLeftDirection, int row) {
        if (isLeftDirection == true) {
            StartPosition = startPosition;
            EndPosition = endPosition;
        }
        else {
            StartPosition = endPosition;
            EndPosition = startPosition;
        }
        this.isLeftDirection = isLeftDirection;
        Row = row;
    }

    public bool IsAtEndPosition(float xValue, float positionOffset = 0f) {
        if (isLeftDirection == true && xValue > EndPosition.x - positionOffset) {
            return true;
        } else if (isLeftDirection == false && xValue < EndPosition.x + positionOffset) {
            return true;
        }
        return false;
    }

    public void FlipDirection() {
        Vector3 startPosition = StartPosition;
        StartPosition = EndPosition;
        EndPosition = startPosition;
        isLeftDirection = !isLeftDirection;
    }

}
