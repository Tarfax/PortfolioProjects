using System.Collections;
using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerControllerCommon : MonoBehaviour
{
    protected PlayerInputAction inputAction;
    protected Rigidbody body = default;
    protected new Transform transform;
    protected Animator anim = null;

    protected float desiredHorizontalMoveSpeed = default;
    protected bool currentlyGrounded = true;
    protected Vector2 aimDirection = Vector2.right;
    protected bool simulatedGravity = true;
    protected int facing = 1; 


    protected IEnumerator jumpRoutine = null;


    

    [Tooltip("The height relative to the character pivot where bullets are fired from (think chest height)")]
    [SerializeField] protected float weaponHeightOffset = 2f;
    [Tooltip("The radial distance relative to the characters weaponHeight where bullets are fired from (think arm length)")]
    [SerializeField] protected float bulletSpawnOffset = 1.5f;

    [Tooltip("A spherecast is used to determine if character is grounded, adjust spherecast length here")]
    [SerializeField] private float groundedCheckDistance = 0.70f;
    [SerializeField] protected float movementSpeed = 5f;
    [SerializeField] protected float jumpSpeed = 4f;
    [Tooltip("The player will move upward while jump is held, up to this maximum duration")]
    [SerializeField] private float maximumJumpTime = 0.3f;
    [Tooltip("Gravity is not applied while jumping up. Balance jumping speed with falling speed for nice game feel")]
    [SerializeField] protected float gravityMultiplier = 1f;

    protected virtual void Awake()
    {
        inputAction = new PlayerInputAction();
        body = gameObject.GetComponent<Rigidbody>();
        body.constraints = RigidbodyConstraints.FreezePositionZ | RigidbodyConstraints.FreezeRotationX | RigidbodyConstraints.FreezeRotationZ | RigidbodyConstraints.FreezeRotationY;
        transform = gameObject.transform;

        anim = GetComponent<Animator>();

        body.useGravity = false;
    }

    protected virtual void FixedUpdate()
    {
        bool previouslyGrounded = currentlyGrounded;
        RaycastHit hit;
        currentlyGrounded = Physics.SphereCast(new Ray(transform.position + new Vector3(0f, 0.5f, 0f), Vector3.down), 0.25f, out hit, groundedCheckDistance);
        if (currentlyGrounded)
        {

        }
        anim.SetBool("Grounded", currentlyGrounded);
        anim.SetFloat("AirborneBlend", Mathf.InverseLerp(-1, 1, body.velocity.y));
    }


    protected void JumpPress(InputAction.CallbackContext context)
    {
        if (currentlyGrounded)
        {
            if (jumpRoutine != null)
            {
                StopCoroutine(jumpRoutine);
            }
            jumpRoutine = jumpTimer();
            StartCoroutine(jumpRoutine);
        }
    }

    protected void Fire(InputAction.CallbackContext context)
    {
        GetComponent<Player_ProjectileAbsorber>().Fire(aimDirection, new Vector3(0, weaponHeightOffset, 0) + (Vector3)aimDirection*bulletSpawnOffset);
    }

    protected void Move(InputAction.CallbackContext context)
    {
        float axis = context.ReadValue<float>();
        desiredHorizontalMoveSpeed = axis * movementSpeed;        
    }

    protected IEnumerator jumpTimer()
    {
        float startTime = Time.time;

        simulatedGravity = false;
        Vector3 updateVelocity = body.velocity;
        updateVelocity.y = jumpSpeed;
        body.velocity = updateVelocity;
        while (Time.time - startTime <= maximumJumpTime 
            && (Keyboard.current.wKey.isPressed || Keyboard.current.spaceKey.isPressed))
        {            
            yield return null; //busy wait for time or key release
        }
        simulatedGravity = true;
    }

}
