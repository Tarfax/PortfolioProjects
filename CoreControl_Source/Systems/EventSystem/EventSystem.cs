using System;
using System.Collections.Generic;

//Written by Mikael Cedergren
namespace MC_Utility {

    public class EventSystem<TEvent> where TEvent : IEvent {

        public static int callbacks;

        private Action<TEvent> eventListener;
        private Dictionary<Type, Action<TEvent>> typeEventListeners;
        private Dictionary<Type, Action<TEvent>> TypeEventListeners {
            get {
                if (Current.typeEventListeners == null) {
                    Current.typeEventListeners = new Dictionary<Type, Action<TEvent>>();
                }
                return Current.typeEventListeners;
            }
        }

        private static EventSystem<TEvent> eventSystem;
        private static EventSystem<TEvent> Current {
            get {
                if (eventSystem == null) {
                    eventSystem = new EventSystem<TEvent>();
                }
                return eventSystem;
            }
        }

        public static void RegisterListener(Action<TEvent> listener) {
            callbacks++;
            Current.eventListener += listener;
        }

        /// <summary>
        /// Use this if you are going to cast and don't want to worry about the Type.
        /// </summary>
        public static void RegisterListener<T>(Action<TEvent> listener) {
            callbacks++;
            if (Current.TypeEventListeners.ContainsKey(typeof(T)) == false) {
                Current.TypeEventListeners.Add(typeof(T), null);
            }
            Current.TypeEventListeners[typeof(T)] += listener;
        }

        public static void UnregisterListener(Action<TEvent> listener) {
            callbacks--;
            Current.eventListener -= listener;
        }

        /// <summary>
        /// Use this if you are going to cast and don't want to worry about the Type.
        /// </summary>
        public static void UnregisterListener<T>(Action<TEvent> listener) {
            callbacks--;
            if (Current.TypeEventListeners.ContainsKey(typeof(T)) == false) {
                Current.TypeEventListeners.Add(typeof(T), null);
            }
            Current.TypeEventListeners[typeof(T)] -= listener;
        }

        public static void FireEvent(TEvent eventInfo) {
            if (Current.eventListener != null) {
                Current.eventListener(eventInfo);
            }
        }

        /// <summary>
        /// Use this if you are going to cast and don't want to worry about the Type.
        /// </summary>
        public static void FireEvent<T>(TEvent eventInfo) {
            if (Current.TypeEventListeners.ContainsKey(typeof(T)) == true) {
                if (Current.TypeEventListeners[typeof(T)] != null) {
                    Current.TypeEventListeners[typeof(T)](eventInfo);
                }
            }
        }

    }
}