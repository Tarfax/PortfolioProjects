using UnityEngine;

public class IcePatch : MonoBehaviour {

    // Start is called before the first frame update
    void Start() {

    }

    // Update is called once per frame
    void Update() {

    }

    private void OnTriggerEnter(Collider other) {
        if (other.CompareTag("V") == true) {
            
            Debug.Log("Make V trip");
        }
    }

}
