using MC_Utility;
using System;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.UI;

[RequireComponent(typeof(CapsuleCollider))]
[RequireComponent(typeof(Rigidbody))]
public class PlayerControllerFreeMouse : PlayerControllerCommon
{

    private Camera mainCam = null;
    private Plane playfieldPlane;
    

    protected override void Awake()
    {
        base.Awake();
        playfieldPlane = new Plane(Vector3.forward, Vector3.zero);
        mainCam = Camera.main;
    }

    private void OnEnable()
    {        
        //inputAction.PlayerCharacterActionMapMouse.Jump.Enable();
        //inputAction.PlayerCharacterActionMapMouse.Move.Enable();
        //inputAction.PlayerCharacterActionMapMouse.Fire.Enable();
        
        inputAction.PlayerCharacterActionMapMouse.Jump.performed += JumpPress;
        inputAction.PlayerCharacterActionMapMouse.Move.performed += Move;
        inputAction.PlayerCharacterActionMapMouse.Fire.performed += Fire;

        EventSystem<GameStartEvent>.RegisterListener(GameStartTrigger);
        EventSystem<GameOverEvent>.RegisterListener(GameOverTrigger);
    }
    
    private void OnDisable()
    {        
        inputAction.PlayerCharacterActionMapMouse.Jump.performed -= JumpPress;
        inputAction.PlayerCharacterActionMapMouse.Move.performed -= Move;
        inputAction.PlayerCharacterActionMapMouse.Fire.performed -= Fire;

        inputAction.PlayerCharacterActionMapMouse.Jump.Disable();
        inputAction.PlayerCharacterActionMapMouse.Move.Disable();
        inputAction.PlayerCharacterActionMapMouse.Fire.Disable();

        EventSystem<GameStartEvent>.UnregisterListener(GameStartTrigger);
        EventSystem<GameOverEvent>.UnregisterListener(GameOverTrigger);
    }


    private void Update()
    {
        Vector2 mouseScreenPos = Mouse.current.position.ReadValue();

        Ray mouseRay = mainCam.ScreenPointToRay(mouseScreenPos);
        float intersectionDistance;
        if (playfieldPlane.Raycast(mouseRay, out intersectionDistance))
        {
            var worldSpaceIntersection = mainCam.transform.position + mouseRay.direction * intersectionDistance;
            aimDirection = (worldSpaceIntersection - (transform.position+new Vector3(0, weaponHeightOffset, 0))).normalized;
            if (aimDirection.x != 0)
            {
                facing = (int)(aimDirection.x / Mathf.Abs(aimDirection.x));
                transform.rotation = Quaternion.Euler(new Vector3(0, 90 * facing));
            }
        } 
        else
        {
            aimDirection = Vector3.up;
        }

        if (desiredHorizontalMoveSpeed > 0 )
         anim.SetInteger("RunningState", 1 * facing);
        else if (desiredHorizontalMoveSpeed < 0)
            anim.SetInteger("RunningState", -1 * facing);
        else
            anim.SetInteger("RunningState", 0);
    }

    protected override void FixedUpdate()
    {        
        base.FixedUpdate();


        Vector3 updateVelocity = body.velocity;
        updateVelocity.x = desiredHorizontalMoveSpeed;
        body.velocity = updateVelocity;

        if (simulatedGravity)
        {
            body.AddForce(Physics.gravity * gravityMultiplier, ForceMode.Acceleration);
        }
    }

    private void OnAnimatorIK(int layerIndex)
    {
        var aimPosition = (Vector3)(aimDirection) * bulletSpawnOffset 
            + (transform.position+new Vector3(0, weaponHeightOffset, 0));
        Debug.DrawLine(transform.position+new Vector3(0, weaponHeightOffset, 0), aimPosition);


        anim.SetIKPositionWeight(AvatarIKGoal.LeftHand, 1f);    
        anim.SetIKPosition(AvatarIKGoal.LeftHand, aimPosition);

        anim.SetLookAtWeight(1f);
        anim.SetLookAtPosition(aimPosition);
    }

    public void GameStartTrigger(GameStartEvent eventInfo)
    {
        inputAction.PlayerCharacterActionMapMouse.Jump.Enable();
        inputAction.PlayerCharacterActionMapMouse.Move.Enable();
        inputAction.PlayerCharacterActionMapMouse.Fire.Enable();
    }

    public void GameOverTrigger(GameOverEvent eventInfo)
    {
        inputAction.PlayerCharacterActionMapMouse.Jump.Disable();
        inputAction.PlayerCharacterActionMapMouse.Move.Disable();
        inputAction.PlayerCharacterActionMapMouse.Fire.Disable();
    }

}
