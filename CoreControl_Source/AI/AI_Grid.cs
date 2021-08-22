using System.Collections.Generic;
using UnityEngine;

public class AI_Grid : MonoBehaviour {

    private static AI_Grid instance;

    [SerializeField] private float lowPoint = default;
    [Range(2, 20)] [SerializeField] private int rows = 5;
    [SerializeField] private float rowWidth = 15f;
    [SerializeField] private float rowDepths = -0.1f;

    private Vector3 lowerPosition;
    private List<AI_Path> paths;

    private AI_Path sniperPath;

    public void Awake() {
        if (instance == null) {
            instance = this;
            PopulatePaths();
        }
    }

#if UNITY_EDITOR
    void OnDrawGizmos() {
        if (Application.isPlaying == false) {
            if (instance == null) {
                instance = this;
            }

            PopulatePaths();

            for (int i = 0; i < rows; i++) {
                AI_Path path = paths[i];
                Gizmos.color = Color.green;
                Gizmos.DrawLine(path.StartPosition, path.EndPosition);
            }

            Gizmos.color = Color.blue;
            Gizmos.DrawLine(sniperPath.StartPosition, sniperPath.EndPosition);
        }
    }
#endif

    public static AI_Path GetSniperPath() {
        return instance.sniperPath;
    }

    public static AI_Path GetPathAtRow(int getRow = 0) {
        if (getRow < instance.paths.Count && getRow >= 0) {
            return instance.paths[getRow];
        }
        return instance.paths[0];
    }

    public static IEnumerable<AI_Path> GetPaths() {
        foreach (AI_Path path in instance.paths) {
            yield return path;
        }
    }

    public static List<AI_Path> GetPathList() {
        if (instance != null && instance.paths != null) {
            return new List<AI_Path>(instance.paths);
        }
        return new List<AI_Path>();
    }

    public static AI_Path GetNearestPath(float yPosition) {
        AI_Path pathToReturn = default;
        float y = float.MaxValue;
        foreach (AI_Path path in instance.paths) {
            float distance = Mathf.Abs(yPosition - path.StartPosition.y);
            if (distance < y) {
                y = distance;
                pathToReturn = path;
            }
        }
        return pathToReturn;
    }

    private void PopulatePaths() {
        paths = new List<AI_Path>();

        Vector3 upperPosition = transform.position;
        lowerPosition = new Vector3(transform.position.x, lowPoint, transform.position.z);

        float yHeight = Mathf.Abs(upperPosition.y - lowerPosition.y) / rows;

        sniperPath = new AI_Path(new Vector3(upperPosition.x, upperPosition.y + yHeight, upperPosition.z), new Vector3(upperPosition.x + rowWidth, upperPosition.y + yHeight, upperPosition.z), true, -1);

        float rowDepthsAdds = 0f;

        for (int i = 0; i < rows; i++) {
            float yValue = upperPosition.y - (yHeight * i);
            Vector3 startPosition = upperPosition;
            startPosition.y = yValue;
            Vector3 endPosition = startPosition;
            endPosition.x += rowWidth;
            startPosition.z += (rowDepthsAdds + rowDepths);
            endPosition.z += (rowDepthsAdds += rowDepths);
            AI_Path path = new AI_Path(startPosition, endPosition, i % 2 == 0, i);
            paths.Add(path);
        }
    }

}
