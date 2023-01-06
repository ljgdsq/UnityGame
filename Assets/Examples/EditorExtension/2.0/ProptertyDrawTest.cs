using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProptertyDrawTest : MonoBehaviour
{
    
    public enum IngredientUnit { Spoon=1, Cup, Bowl, Piece }

    [Serializable]
    public class Ingredient
    {
        public string name;
        public int amount = 1;
        public IngredientUnit unit;
    }
    
    public Ingredient potionResult;
    public Ingredient[] potionIngredients;
    
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
