using System;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.UI;


/// <summary>
/// DO NOT USE THIS
/// </summary>
[RequireComponent(typeof(CapsuleCollider))]
[RequireComponent(typeof(Rigidbody))]
public class PlayerController : MonoBehaviour
{
    private PlayerInputAction inputAction;

    private Rigidbody body = default;
    private new Transform transform;

    private const float SensitivityScale = 0.0001f;

    private bool grounded = true;
    private float desiredHorizontalMoveSpeed = default;
    private float aimAngle = 0;
    private float aimAxis1D = 0;
    private float mouseArcNormalizedRange = 0.5f;
    private float facing = 1; // TODO use facing direction with mouse aiming

    private Vector2 aimDirection = Vector2.right;

    //[SerializeField] private float aimSpeed = 5f;
    [Range(0f, 10f)]
    [SerializeField] private float mouseSensitivity = 1f;
    [SerializeField] private float movementSpeed = 5f;
    [SerializeField] private float jumpImpulse = 10f;
    [SerializeField] private float groundedCheckDistance = 1.1f;
    [SerializeField] private Transform crosshair = null;
    [SerializeField] private float crosshairDistance = 3f;

    private void Awake()
    {
        inputAction = new PlayerInputAction();
        body = gameObject.GetComponent<Rigidbody>();
        body.constraints = RigidbodyConstraints.FreezePositionZ | RigidbodyConstraints.FreezeRotationX | RigidbodyConstraints.FreezeRotationZ;
        transform = gameObject.transform;

        //TODO put cursor stuff in some game manager!
        Cursor.lockState = CursorLockMode.Locked;
        Cursor.visible = false;
    }

    private void OnEnable()
    {
        //inputAction.PlayerCharacterActionMap.Jump.Enable();
        //inputAction.PlayerCharacterActionMap.Move.Enable();
        //inputAction.PlayerCharacterActionMap.Fire.Enable();
        //inputAction.PlayerCharacterActionMap.KeyboardAim.Enable();
        //
        //inputAction.PlayerCharacterActionMap.Jump.performed += Jump;
        //inputAction.PlayerCharacterActionMap.Move.performed += Move;
        //inputAction.PlayerCharacterActionMap.Fire.performed += Fire;
        //inputAction.PlayerCharacterActionMap.KeyboardAim.performed += KeyboardAim;

        inputAction.PlayerCharacterActionMapMouse.Jump.Enable();
        inputAction.PlayerCharacterActionMapMouse.Move.Enable();
        inputAction.PlayerCharacterActionMapMouse.Fire.Enable();
        inputAction.PlayerCharacterActionMapMouse.MouseAim.Enable();
        
        inputAction.PlayerCharacterActionMapMouse.Jump.performed += Jump;
        inputAction.PlayerCharacterActionMapMouse.Move.performed += Move;
        inputAction.PlayerCharacterActionMapMouse.Fire.performed += Fire;
        inputAction.PlayerCharacterActionMapMouse.MouseAim.performed += MouseAim;
    }

    
    private void OnDisable()
    {
        //inputAction.PlayerCharacterActionMap.Jump.performed -= Jump;
        //inputAction.PlayerCharacterActionMap.Move.performed -= Move;
        //inputAction.PlayerCharacterActionMap.Fire.performed -= Fire;
        //inputAction.PlayerCharacterActionMap.KeyboardAim.performed -= KeyboardAim;
        //
        //inputAction.PlayerCharacterActionMap.Jump.Disable();
        //inputAction.PlayerCharacterActionMap.Move.Disable();
        //inputAction.PlayerCharacterActionMap.Fire.Disable();
        //inputAction.PlayerCharacterActionMap.KeyboardAim.Disable();

        inputAction.PlayerCharacterActionMapMouse.Jump.Disable();
        inputAction.PlayerCharacterActionMapMouse.Move.Disable();
        inputAction.PlayerCharacterActionMapMouse.Fire.Disable();
        inputAction.PlayerCharacterActionMapMouse.MouseAim.Disable();

        inputAction.PlayerCharacterActionMapMouse.Jump.performed -= Jump;
        inputAction.PlayerCharacterActionMapMouse.Move.performed -= Move;
        inputAction.PlayerCharacterActionMapMouse.Fire.performed -= Fire;
        inputAction.PlayerCharacterActionMapMouse.MouseAim.performed -= MouseAim;
    }

    private void Update()
    {
        //aimAngle += aimSpeed * Time.deltaTime * aimAxis1D;
        //aimAngle = Mathf.Clamp(aimAngle, 0, 0.25f * 2f * Mathf.PI);        
        //aim = new Vector2(Mathf.Cos(aimAngle)*facing, Mathf.Sin(aimAngle));
        //aim = aim * crosshairDistance;

        aimDirection = new Vector2(Mathf.Cos(mouseArcNormalizedRange*Mathf.PI), Mathf.Sin(mouseArcNormalizedRange * Mathf.PI));
        aimDirection = aimDirection * crosshairDistance;

        crosshair.localPosition = new Vector3(aimDirection.x, aimDirection.y, 0);

        
    }

    private void FixedUpdate()
    {
        grounded = Physics.Raycast(new Ray(transform.position, Vector3.down), groundedCheckDistance);

        Vector3 updateVelocity = body.velocity;
        updateVelocity.x = desiredHorizontalMoveSpeed;
        body.velocity = updateVelocity;
    }

    private void OnValidate()
    {
        if (crosshair == null)
        {
            Debug.LogWarning("OnValidate: PlayerController requires a child object to use as crosshair.");
        }
        else if (crosshair.parent == transform)
        {
            Debug.LogWarning("OnValidate: PlayerController's crosshair reference was not a child of PlayerController!");
        }
    }

    private void Jump(InputAction.CallbackContext context)
    {
        if (grounded)
        {
            body.AddForce(Vector3.up*jumpImpulse, ForceMode.Impulse);
        }
    }

    private void Move(InputAction.CallbackContext context)
    {
        float axis = context.ReadValue<float>();
        desiredHorizontalMoveSpeed = axis * movementSpeed;
        if (axis != 0)
        {
            facing = axis/Mathf.Abs(axis); //normalizing to -1,1, unnecessary?
        }
    }

    private void KeyboardAim(InputAction.CallbackContext context)
    {
        aimAxis1D = context.ReadValue<float>();
    }

    private void Fire(InputAction.CallbackContext context)
    {
        GetComponent<Player_ProjectileAbsorber>().Fire(aimDirection, Vector2.up * 0.5f); //todo warning dirty hack
    }

    private void MouseAim(InputAction.CallbackContext context)
    {
        Vector2 mouseDelta = context.ReadValue<Vector2>();
        mouseArcNormalizedRange -= mouseDelta.x * mouseSensitivity * SensitivityScale;
        mouseArcNormalizedRange = Mathf.Clamp(mouseArcNormalizedRange, 0, 1);
    }

}
