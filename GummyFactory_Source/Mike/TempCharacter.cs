using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TempCharacter : MonoBehaviour {

    private ObjectType objectType;
    public ObjectType ObjectType { get => objectType; private set { objectType = value; } }

    private void Start() {
        objectType = ObjectType.V;
    }

}
