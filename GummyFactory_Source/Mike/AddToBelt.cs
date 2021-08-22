using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AddToBelt : MonoBehaviour
{

    public Belt belt;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.B) == true) {
            belt.AttachObject(gameObject);
        }
    }
}
