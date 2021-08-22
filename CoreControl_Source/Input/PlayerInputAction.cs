// GENERATED AUTOMATICALLY FROM 'Assets/_Game/Scripts/Input/PlayerInputAction.inputactions'

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.InputSystem;
using UnityEngine.InputSystem.Utilities;

public class @PlayerInputAction : IInputActionCollection, IDisposable
{
    public InputActionAsset asset { get; }
    public @PlayerInputAction()
    {
        asset = InputActionAsset.FromJson(@"{
    ""name"": ""PlayerInputAction"",
    ""maps"": [
        {
            ""name"": ""PlayerCharacterActionMap"",
            ""id"": ""466e8526-cc1d-41ff-8082-4e2ab9da7329"",
            ""actions"": [
                {
                    ""name"": ""Jump"",
                    ""type"": ""Button"",
                    ""id"": ""230a4f3e-9dee-4c54-bfb8-887e89736a3c"",
                    ""expectedControlType"": ""Button"",
                    ""processors"": """",
                    ""interactions"": """"
                },
                {
                    ""name"": ""Move"",
                    ""type"": ""PassThrough"",
                    ""id"": ""b4f4db60-aba0-4132-9b88-ed461c7040e6"",
                    ""expectedControlType"": ""Axis"",
                    ""processors"": """",
                    ""interactions"": """"
                },
                {
                    ""name"": ""KeyboardAim"",
                    ""type"": ""PassThrough"",
                    ""id"": ""04784e46-6dae-427c-8ce9-eef903c91ea0"",
                    ""expectedControlType"": ""Button"",
                    ""processors"": """",
                    ""interactions"": """"
                },
                {
                    ""name"": ""Fire"",
                    ""type"": ""Button"",
                    ""id"": ""dbc85b65-9c96-4835-aa15-610ba69d6e96"",
                    ""expectedControlType"": ""Button"",
                    ""processors"": """",
                    ""interactions"": """"
                }
            ],
            ""bindings"": [
                {
                    ""name"": """",
                    ""id"": ""574a4d35-34e2-4c9f-a52a-a053df0a8023"",
                    ""path"": ""<Keyboard>/leftShift"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""Jump"",
                    ""isComposite"": false,
                    ""isPartOfComposite"": false
                },
                {
                    ""name"": """",
                    ""id"": ""9a68faed-69f1-48cd-8483-01eb0c84909f"",
                    ""path"": ""<Keyboard>/space"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""Jump"",
                    ""isComposite"": false,
                    ""isPartOfComposite"": false
                },
                {
                    ""name"": ""1D Axis"",
                    ""id"": ""649c348f-9d80-47d6-aa01-bef37b17e75a"",
                    ""path"": ""1DAxis"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""Move"",
                    ""isComposite"": true,
                    ""isPartOfComposite"": false
                },
                {
                    ""name"": ""negative"",
                    ""id"": ""93057089-1645-4e3a-a30d-8397109ea390"",
                    ""path"": ""<Keyboard>/leftArrow"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""Move"",
                    ""isComposite"": false,
                    ""isPartOfComposite"": true
                },
                {
                    ""name"": ""positive"",
                    ""id"": ""5fb21cb8-efa1-456b-aa6c-1abf48bfbb43"",
                    ""path"": ""<Keyboard>/rightArrow"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""Move"",
                    ""isComposite"": false,
                    ""isPartOfComposite"": true
                },
                {
                    ""name"": ""1D Axis"",
                    ""id"": ""d8214ffe-d5f3-46dc-9468-0548819af700"",
                    ""path"": ""1DAxis"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""KeyboardAim"",
                    ""isComposite"": true,
                    ""isPartOfComposite"": false
                },
                {
                    ""name"": ""positive"",
                    ""id"": ""d00e8574-4dac-4f36-a1bc-1badbcea7f13"",
                    ""path"": ""<Keyboard>/upArrow"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""KeyboardAim"",
                    ""isComposite"": false,
                    ""isPartOfComposite"": true
                },
                {
                    ""name"": ""negative"",
                    ""id"": ""76412e4d-e125-4725-97df-39c7066a13ff"",
                    ""path"": ""<Keyboard>/downArrow"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""KeyboardAim"",
                    ""isComposite"": false,
                    ""isPartOfComposite"": true
                },
                {
                    ""name"": """",
                    ""id"": ""f92d614e-f48e-4700-a10c-952586e328bc"",
                    ""path"": ""<Keyboard>/leftCtrl"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""Fire"",
                    ""isComposite"": false,
                    ""isPartOfComposite"": false
                }
            ]
        },
        {
            ""name"": ""PlayerCharacterActionMapMouse"",
            ""id"": ""d47231bc-22d9-49ea-88ee-d74626626e01"",
            ""actions"": [
                {
                    ""name"": ""Jump"",
                    ""type"": ""Button"",
                    ""id"": ""62ccc4f9-e205-4358-934f-6d1c8b675683"",
                    ""expectedControlType"": ""Button"",
                    ""processors"": """",
                    ""interactions"": """"
                },
                {
                    ""name"": ""Fire"",
                    ""type"": ""Button"",
                    ""id"": ""af5f6f98-7dd9-4f26-bd79-b833285cdec7"",
                    ""expectedControlType"": ""Button"",
                    ""processors"": """",
                    ""interactions"": """"
                },
                {
                    ""name"": ""MouseAim"",
                    ""type"": ""PassThrough"",
                    ""id"": ""3ead57f7-d0f3-414d-a76b-800265946fc5"",
                    ""expectedControlType"": ""Vector2"",
                    ""processors"": """",
                    ""interactions"": """"
                },
                {
                    ""name"": ""Move"",
                    ""type"": ""PassThrough"",
                    ""id"": ""427ea60e-b659-4cef-a53f-7e4cb441d345"",
                    ""expectedControlType"": ""Axis"",
                    ""processors"": """",
                    ""interactions"": """"
                }
            ],
            ""bindings"": [
                {
                    ""name"": """",
                    ""id"": ""21cf2865-d41e-4b91-96c0-55845f62fb60"",
                    ""path"": ""<Keyboard>/w"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""Jump"",
                    ""isComposite"": false,
                    ""isPartOfComposite"": false
                },
                {
                    ""name"": """",
                    ""id"": ""93cd3895-85a4-4f28-a07c-c9c4472494fa"",
                    ""path"": ""<Keyboard>/space"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""Jump"",
                    ""isComposite"": false,
                    ""isPartOfComposite"": false
                },
                {
                    ""name"": ""1D Axis"",
                    ""id"": ""fd32bda3-d910-4864-bdec-2eeef98ac33b"",
                    ""path"": ""1DAxis"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""Move"",
                    ""isComposite"": true,
                    ""isPartOfComposite"": false
                },
                {
                    ""name"": ""negative"",
                    ""id"": ""2a3f0bc7-c704-48bc-bb3e-d177d6d06b33"",
                    ""path"": ""<Keyboard>/a"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""Move"",
                    ""isComposite"": false,
                    ""isPartOfComposite"": true
                },
                {
                    ""name"": ""positive"",
                    ""id"": ""2567e6a1-eca4-4bce-a0be-c76a93042ee7"",
                    ""path"": ""<Keyboard>/d"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""Move"",
                    ""isComposite"": false,
                    ""isPartOfComposite"": true
                },
                {
                    ""name"": """",
                    ""id"": ""53548672-eb16-4fcf-8cb4-7d50d3e9aeb5"",
                    ""path"": ""<Mouse>/leftButton"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""Fire"",
                    ""isComposite"": false,
                    ""isPartOfComposite"": false
                },
                {
                    ""name"": """",
                    ""id"": ""bd41ea76-08c2-492e-9e79-2b5d4c31d8fd"",
                    ""path"": ""<Mouse>/delta"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""MouseAim"",
                    ""isComposite"": false,
                    ""isPartOfComposite"": false
                }
            ]
        },
        {
            ""name"": ""PlayerCharacterActionMapFreeMouse"",
            ""id"": ""80a0dc67-1776-4b6e-a7a7-d7ca33f36155"",
            ""actions"": [
                {
                    ""name"": ""Move"",
                    ""type"": ""PassThrough"",
                    ""id"": ""57065ea2-0d42-49bb-b623-a0485c320de6"",
                    ""expectedControlType"": ""Axis"",
                    ""processors"": """",
                    ""interactions"": """"
                },
                {
                    ""name"": ""MouseAim"",
                    ""type"": ""PassThrough"",
                    ""id"": ""6613e2b7-6dc1-49cb-b7ef-778b014c189e"",
                    ""expectedControlType"": ""Vector2"",
                    ""processors"": """",
                    ""interactions"": """"
                },
                {
                    ""name"": ""Fire"",
                    ""type"": ""Button"",
                    ""id"": ""6ba35529-e26b-4bd8-a965-1ee66f831186"",
                    ""expectedControlType"": ""Button"",
                    ""processors"": """",
                    ""interactions"": """"
                },
                {
                    ""name"": ""Jump"",
                    ""type"": ""Button"",
                    ""id"": ""8539d107-cc94-4358-86f3-371d87e3eb97"",
                    ""expectedControlType"": ""Button"",
                    ""processors"": """",
                    ""interactions"": """"
                }
            ],
            ""bindings"": [
                {
                    ""name"": """",
                    ""id"": ""5075b277-fbd4-413a-9bff-1ba850a3b0e4"",
                    ""path"": ""<Keyboard>/w"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""Jump"",
                    ""isComposite"": false,
                    ""isPartOfComposite"": false
                },
                {
                    ""name"": """",
                    ""id"": ""97469a74-036d-4b4e-8452-6bfc13e2f69a"",
                    ""path"": ""<Keyboard>/space"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""Jump"",
                    ""isComposite"": false,
                    ""isPartOfComposite"": false
                },
                {
                    ""name"": """",
                    ""id"": ""1a607ff4-b672-4e14-8d4c-7724bbebdc2d"",
                    ""path"": ""<Mouse>/leftButton"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""Fire"",
                    ""isComposite"": false,
                    ""isPartOfComposite"": false
                },
                {
                    ""name"": """",
                    ""id"": ""c760dba2-cebe-49b1-bf6e-e406c52bacaf"",
                    ""path"": ""<Mouse>/delta"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""MouseAim"",
                    ""isComposite"": false,
                    ""isPartOfComposite"": false
                },
                {
                    ""name"": ""1D Axis"",
                    ""id"": ""7aefde52-45a1-4a82-a4a2-5c34b47c15b1"",
                    ""path"": ""1DAxis"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""Move"",
                    ""isComposite"": true,
                    ""isPartOfComposite"": false
                },
                {
                    ""name"": ""negative"",
                    ""id"": ""c3a59148-fe29-4623-b2f1-480dcab63c13"",
                    ""path"": ""<Keyboard>/a"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""Move"",
                    ""isComposite"": false,
                    ""isPartOfComposite"": true
                },
                {
                    ""name"": ""positive"",
                    ""id"": ""222989f7-74ab-4a81-83bf-4ff45437ea6b"",
                    ""path"": ""<Keyboard>/d"",
                    ""interactions"": """",
                    ""processors"": """",
                    ""groups"": """",
                    ""action"": ""Move"",
                    ""isComposite"": false,
                    ""isPartOfComposite"": true
                }
            ]
        }
    ],
    ""controlSchemes"": []
}");
        // PlayerCharacterActionMap
        m_PlayerCharacterActionMap = asset.FindActionMap("PlayerCharacterActionMap", throwIfNotFound: true);
        m_PlayerCharacterActionMap_Jump = m_PlayerCharacterActionMap.FindAction("Jump", throwIfNotFound: true);
        m_PlayerCharacterActionMap_Move = m_PlayerCharacterActionMap.FindAction("Move", throwIfNotFound: true);
        m_PlayerCharacterActionMap_KeyboardAim = m_PlayerCharacterActionMap.FindAction("KeyboardAim", throwIfNotFound: true);
        m_PlayerCharacterActionMap_Fire = m_PlayerCharacterActionMap.FindAction("Fire", throwIfNotFound: true);
        // PlayerCharacterActionMapMouse
        m_PlayerCharacterActionMapMouse = asset.FindActionMap("PlayerCharacterActionMapMouse", throwIfNotFound: true);
        m_PlayerCharacterActionMapMouse_Jump = m_PlayerCharacterActionMapMouse.FindAction("Jump", throwIfNotFound: true);
        m_PlayerCharacterActionMapMouse_Fire = m_PlayerCharacterActionMapMouse.FindAction("Fire", throwIfNotFound: true);
        m_PlayerCharacterActionMapMouse_MouseAim = m_PlayerCharacterActionMapMouse.FindAction("MouseAim", throwIfNotFound: true);
        m_PlayerCharacterActionMapMouse_Move = m_PlayerCharacterActionMapMouse.FindAction("Move", throwIfNotFound: true);
        // PlayerCharacterActionMapFreeMouse
        m_PlayerCharacterActionMapFreeMouse = asset.FindActionMap("PlayerCharacterActionMapFreeMouse", throwIfNotFound: true);
        m_PlayerCharacterActionMapFreeMouse_Move = m_PlayerCharacterActionMapFreeMouse.FindAction("Move", throwIfNotFound: true);
        m_PlayerCharacterActionMapFreeMouse_MouseAim = m_PlayerCharacterActionMapFreeMouse.FindAction("MouseAim", throwIfNotFound: true);
        m_PlayerCharacterActionMapFreeMouse_Fire = m_PlayerCharacterActionMapFreeMouse.FindAction("Fire", throwIfNotFound: true);
        m_PlayerCharacterActionMapFreeMouse_Jump = m_PlayerCharacterActionMapFreeMouse.FindAction("Jump", throwIfNotFound: true);
    }

    public void Dispose()
    {
        UnityEngine.Object.Destroy(asset);
    }

    public InputBinding? bindingMask
    {
        get => asset.bindingMask;
        set => asset.bindingMask = value;
    }

    public ReadOnlyArray<InputDevice>? devices
    {
        get => asset.devices;
        set => asset.devices = value;
    }

    public ReadOnlyArray<InputControlScheme> controlSchemes => asset.controlSchemes;

    public bool Contains(InputAction action)
    {
        return asset.Contains(action);
    }

    public IEnumerator<InputAction> GetEnumerator()
    {
        return asset.GetEnumerator();
    }

    IEnumerator IEnumerable.GetEnumerator()
    {
        return GetEnumerator();
    }

    public void Enable()
    {
        asset.Enable();
    }

    public void Disable()
    {
        asset.Disable();
    }

    // PlayerCharacterActionMap
    private readonly InputActionMap m_PlayerCharacterActionMap;
    private IPlayerCharacterActionMapActions m_PlayerCharacterActionMapActionsCallbackInterface;
    private readonly InputAction m_PlayerCharacterActionMap_Jump;
    private readonly InputAction m_PlayerCharacterActionMap_Move;
    private readonly InputAction m_PlayerCharacterActionMap_KeyboardAim;
    private readonly InputAction m_PlayerCharacterActionMap_Fire;
    public struct PlayerCharacterActionMapActions
    {
        private @PlayerInputAction m_Wrapper;
        public PlayerCharacterActionMapActions(@PlayerInputAction wrapper) { m_Wrapper = wrapper; }
        public InputAction @Jump => m_Wrapper.m_PlayerCharacterActionMap_Jump;
        public InputAction @Move => m_Wrapper.m_PlayerCharacterActionMap_Move;
        public InputAction @KeyboardAim => m_Wrapper.m_PlayerCharacterActionMap_KeyboardAim;
        public InputAction @Fire => m_Wrapper.m_PlayerCharacterActionMap_Fire;
        public InputActionMap Get() { return m_Wrapper.m_PlayerCharacterActionMap; }
        public void Enable() { Get().Enable(); }
        public void Disable() { Get().Disable(); }
        public bool enabled => Get().enabled;
        public static implicit operator InputActionMap(PlayerCharacterActionMapActions set) { return set.Get(); }
        public void SetCallbacks(IPlayerCharacterActionMapActions instance)
        {
            if (m_Wrapper.m_PlayerCharacterActionMapActionsCallbackInterface != null)
            {
                @Jump.started -= m_Wrapper.m_PlayerCharacterActionMapActionsCallbackInterface.OnJump;
                @Jump.performed -= m_Wrapper.m_PlayerCharacterActionMapActionsCallbackInterface.OnJump;
                @Jump.canceled -= m_Wrapper.m_PlayerCharacterActionMapActionsCallbackInterface.OnJump;
                @Move.started -= m_Wrapper.m_PlayerCharacterActionMapActionsCallbackInterface.OnMove;
                @Move.performed -= m_Wrapper.m_PlayerCharacterActionMapActionsCallbackInterface.OnMove;
                @Move.canceled -= m_Wrapper.m_PlayerCharacterActionMapActionsCallbackInterface.OnMove;
                @KeyboardAim.started -= m_Wrapper.m_PlayerCharacterActionMapActionsCallbackInterface.OnKeyboardAim;
                @KeyboardAim.performed -= m_Wrapper.m_PlayerCharacterActionMapActionsCallbackInterface.OnKeyboardAim;
                @KeyboardAim.canceled -= m_Wrapper.m_PlayerCharacterActionMapActionsCallbackInterface.OnKeyboardAim;
                @Fire.started -= m_Wrapper.m_PlayerCharacterActionMapActionsCallbackInterface.OnFire;
                @Fire.performed -= m_Wrapper.m_PlayerCharacterActionMapActionsCallbackInterface.OnFire;
                @Fire.canceled -= m_Wrapper.m_PlayerCharacterActionMapActionsCallbackInterface.OnFire;
            }
            m_Wrapper.m_PlayerCharacterActionMapActionsCallbackInterface = instance;
            if (instance != null)
            {
                @Jump.started += instance.OnJump;
                @Jump.performed += instance.OnJump;
                @Jump.canceled += instance.OnJump;
                @Move.started += instance.OnMove;
                @Move.performed += instance.OnMove;
                @Move.canceled += instance.OnMove;
                @KeyboardAim.started += instance.OnKeyboardAim;
                @KeyboardAim.performed += instance.OnKeyboardAim;
                @KeyboardAim.canceled += instance.OnKeyboardAim;
                @Fire.started += instance.OnFire;
                @Fire.performed += instance.OnFire;
                @Fire.canceled += instance.OnFire;
            }
        }
    }
    public PlayerCharacterActionMapActions @PlayerCharacterActionMap => new PlayerCharacterActionMapActions(this);

    // PlayerCharacterActionMapMouse
    private readonly InputActionMap m_PlayerCharacterActionMapMouse;
    private IPlayerCharacterActionMapMouseActions m_PlayerCharacterActionMapMouseActionsCallbackInterface;
    private readonly InputAction m_PlayerCharacterActionMapMouse_Jump;
    private readonly InputAction m_PlayerCharacterActionMapMouse_Fire;
    private readonly InputAction m_PlayerCharacterActionMapMouse_MouseAim;
    private readonly InputAction m_PlayerCharacterActionMapMouse_Move;
    public struct PlayerCharacterActionMapMouseActions
    {
        private @PlayerInputAction m_Wrapper;
        public PlayerCharacterActionMapMouseActions(@PlayerInputAction wrapper) { m_Wrapper = wrapper; }
        public InputAction @Jump => m_Wrapper.m_PlayerCharacterActionMapMouse_Jump;
        public InputAction @Fire => m_Wrapper.m_PlayerCharacterActionMapMouse_Fire;
        public InputAction @MouseAim => m_Wrapper.m_PlayerCharacterActionMapMouse_MouseAim;
        public InputAction @Move => m_Wrapper.m_PlayerCharacterActionMapMouse_Move;
        public InputActionMap Get() { return m_Wrapper.m_PlayerCharacterActionMapMouse; }
        public void Enable() { Get().Enable(); }
        public void Disable() { Get().Disable(); }
        public bool enabled => Get().enabled;
        public static implicit operator InputActionMap(PlayerCharacterActionMapMouseActions set) { return set.Get(); }
        public void SetCallbacks(IPlayerCharacterActionMapMouseActions instance)
        {
            if (m_Wrapper.m_PlayerCharacterActionMapMouseActionsCallbackInterface != null)
            {
                @Jump.started -= m_Wrapper.m_PlayerCharacterActionMapMouseActionsCallbackInterface.OnJump;
                @Jump.performed -= m_Wrapper.m_PlayerCharacterActionMapMouseActionsCallbackInterface.OnJump;
                @Jump.canceled -= m_Wrapper.m_PlayerCharacterActionMapMouseActionsCallbackInterface.OnJump;
                @Fire.started -= m_Wrapper.m_PlayerCharacterActionMapMouseActionsCallbackInterface.OnFire;
                @Fire.performed -= m_Wrapper.m_PlayerCharacterActionMapMouseActionsCallbackInterface.OnFire;
                @Fire.canceled -= m_Wrapper.m_PlayerCharacterActionMapMouseActionsCallbackInterface.OnFire;
                @MouseAim.started -= m_Wrapper.m_PlayerCharacterActionMapMouseActionsCallbackInterface.OnMouseAim;
                @MouseAim.performed -= m_Wrapper.m_PlayerCharacterActionMapMouseActionsCallbackInterface.OnMouseAim;
                @MouseAim.canceled -= m_Wrapper.m_PlayerCharacterActionMapMouseActionsCallbackInterface.OnMouseAim;
                @Move.started -= m_Wrapper.m_PlayerCharacterActionMapMouseActionsCallbackInterface.OnMove;
                @Move.performed -= m_Wrapper.m_PlayerCharacterActionMapMouseActionsCallbackInterface.OnMove;
                @Move.canceled -= m_Wrapper.m_PlayerCharacterActionMapMouseActionsCallbackInterface.OnMove;
            }
            m_Wrapper.m_PlayerCharacterActionMapMouseActionsCallbackInterface = instance;
            if (instance != null)
            {
                @Jump.started += instance.OnJump;
                @Jump.performed += instance.OnJump;
                @Jump.canceled += instance.OnJump;
                @Fire.started += instance.OnFire;
                @Fire.performed += instance.OnFire;
                @Fire.canceled += instance.OnFire;
                @MouseAim.started += instance.OnMouseAim;
                @MouseAim.performed += instance.OnMouseAim;
                @MouseAim.canceled += instance.OnMouseAim;
                @Move.started += instance.OnMove;
                @Move.performed += instance.OnMove;
                @Move.canceled += instance.OnMove;
            }
        }
    }
    public PlayerCharacterActionMapMouseActions @PlayerCharacterActionMapMouse => new PlayerCharacterActionMapMouseActions(this);

    // PlayerCharacterActionMapFreeMouse
    private readonly InputActionMap m_PlayerCharacterActionMapFreeMouse;
    private IPlayerCharacterActionMapFreeMouseActions m_PlayerCharacterActionMapFreeMouseActionsCallbackInterface;
    private readonly InputAction m_PlayerCharacterActionMapFreeMouse_Move;
    private readonly InputAction m_PlayerCharacterActionMapFreeMouse_MouseAim;
    private readonly InputAction m_PlayerCharacterActionMapFreeMouse_Fire;
    private readonly InputAction m_PlayerCharacterActionMapFreeMouse_Jump;
    public struct PlayerCharacterActionMapFreeMouseActions
    {
        private @PlayerInputAction m_Wrapper;
        public PlayerCharacterActionMapFreeMouseActions(@PlayerInputAction wrapper) { m_Wrapper = wrapper; }
        public InputAction @Move => m_Wrapper.m_PlayerCharacterActionMapFreeMouse_Move;
        public InputAction @MouseAim => m_Wrapper.m_PlayerCharacterActionMapFreeMouse_MouseAim;
        public InputAction @Fire => m_Wrapper.m_PlayerCharacterActionMapFreeMouse_Fire;
        public InputAction @Jump => m_Wrapper.m_PlayerCharacterActionMapFreeMouse_Jump;
        public InputActionMap Get() { return m_Wrapper.m_PlayerCharacterActionMapFreeMouse; }
        public void Enable() { Get().Enable(); }
        public void Disable() { Get().Disable(); }
        public bool enabled => Get().enabled;
        public static implicit operator InputActionMap(PlayerCharacterActionMapFreeMouseActions set) { return set.Get(); }
        public void SetCallbacks(IPlayerCharacterActionMapFreeMouseActions instance)
        {
            if (m_Wrapper.m_PlayerCharacterActionMapFreeMouseActionsCallbackInterface != null)
            {
                @Move.started -= m_Wrapper.m_PlayerCharacterActionMapFreeMouseActionsCallbackInterface.OnMove;
                @Move.performed -= m_Wrapper.m_PlayerCharacterActionMapFreeMouseActionsCallbackInterface.OnMove;
                @Move.canceled -= m_Wrapper.m_PlayerCharacterActionMapFreeMouseActionsCallbackInterface.OnMove;
                @MouseAim.started -= m_Wrapper.m_PlayerCharacterActionMapFreeMouseActionsCallbackInterface.OnMouseAim;
                @MouseAim.performed -= m_Wrapper.m_PlayerCharacterActionMapFreeMouseActionsCallbackInterface.OnMouseAim;
                @MouseAim.canceled -= m_Wrapper.m_PlayerCharacterActionMapFreeMouseActionsCallbackInterface.OnMouseAim;
                @Fire.started -= m_Wrapper.m_PlayerCharacterActionMapFreeMouseActionsCallbackInterface.OnFire;
                @Fire.performed -= m_Wrapper.m_PlayerCharacterActionMapFreeMouseActionsCallbackInterface.OnFire;
                @Fire.canceled -= m_Wrapper.m_PlayerCharacterActionMapFreeMouseActionsCallbackInterface.OnFire;
                @Jump.started -= m_Wrapper.m_PlayerCharacterActionMapFreeMouseActionsCallbackInterface.OnJump;
                @Jump.performed -= m_Wrapper.m_PlayerCharacterActionMapFreeMouseActionsCallbackInterface.OnJump;
                @Jump.canceled -= m_Wrapper.m_PlayerCharacterActionMapFreeMouseActionsCallbackInterface.OnJump;
            }
            m_Wrapper.m_PlayerCharacterActionMapFreeMouseActionsCallbackInterface = instance;
            if (instance != null)
            {
                @Move.started += instance.OnMove;
                @Move.performed += instance.OnMove;
                @Move.canceled += instance.OnMove;
                @MouseAim.started += instance.OnMouseAim;
                @MouseAim.performed += instance.OnMouseAim;
                @MouseAim.canceled += instance.OnMouseAim;
                @Fire.started += instance.OnFire;
                @Fire.performed += instance.OnFire;
                @Fire.canceled += instance.OnFire;
                @Jump.started += instance.OnJump;
                @Jump.performed += instance.OnJump;
                @Jump.canceled += instance.OnJump;
            }
        }
    }
    public PlayerCharacterActionMapFreeMouseActions @PlayerCharacterActionMapFreeMouse => new PlayerCharacterActionMapFreeMouseActions(this);
    public interface IPlayerCharacterActionMapActions
    {
        void OnJump(InputAction.CallbackContext context);
        void OnMove(InputAction.CallbackContext context);
        void OnKeyboardAim(InputAction.CallbackContext context);
        void OnFire(InputAction.CallbackContext context);
    }
    public interface IPlayerCharacterActionMapMouseActions
    {
        void OnJump(InputAction.CallbackContext context);
        void OnFire(InputAction.CallbackContext context);
        void OnMouseAim(InputAction.CallbackContext context);
        void OnMove(InputAction.CallbackContext context);
    }
    public interface IPlayerCharacterActionMapFreeMouseActions
    {
        void OnMove(InputAction.CallbackContext context);
        void OnMouseAim(InputAction.CallbackContext context);
        void OnFire(InputAction.CallbackContext context);
        void OnJump(InputAction.CallbackContext context);
    }
}
