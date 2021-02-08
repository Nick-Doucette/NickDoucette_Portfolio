/* DESCRIPTION COMMENT FOR PORTFOLLIO:
 * 
 * This script was created to handle two different mechanics for the player.
 * 
 * It holds all the player weapons called "modules" in a List. 
 * It also handles swapping between these weapons and picking up new weapons dropped by the enemy.
 * 
 * For reference, modules themselves are handled in separate scripts created by one of the other developers on this game.
 * This scripts simply deals with modules the player can use.
 */


//General Unity library references
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

//This class is referenced by Unity so it can be added to a game object in the editor
public class PlayerModuleHolder : MonoBehaviour
{
    //Serialized Field allows the variable declared under it to be seen in the Unity editor for debug purposes
    //All private variables are declared here
    [SerializeField]
    private GameObject currentModule;                                                                               //stores our currently selected module

    [SerializeField]
    private List<GameObject> modules;                                                                               //stores all modules the player currently has

    private int activeModule = 0;                                                                                   //used as index to keep track of the currently used module
    private bool haveCheck = false;                                                                                 //boolean used in module pickup detection
    private string tempString;                                                                                      //temporary string variably used in module pickup detection
    private GameObject weaponSprite;                                                                                //Unity GameObject that holds our current module sprite object

    //Unity Sprite objects that hold the sprites for our modules
    private Sprite gunSprite;                                                                                       
    private Sprite spreadSprite;                                                                                    
    private Sprite rocketSprite;                                                                                    
    private Sprite shieldSprite;

    //Start is called before the first frame update
    void Start()
    {
        modules = new List<GameObject>();                                                                           //create our module list

        //find all our required game obects in the scene
        modules.Add(currentModule);
        weaponSprite = GameObject.Find("WeaponSprite");
        
        //load all of our sprites
        gunSprite = Resources.Load<Sprite>("mod_blaster");
        rocketSprite = Resources.Load<Sprite>("mod_rpg");
        spreadSprite = Resources.Load<Sprite>("mod_spread");
        shieldSprite = Resources.Load<Sprite>("mod_shield");
    }

    //update is called one time for every frame
    void Update()
    {
        //cycle active module using scroll wheel input
        if (Input.mouseScrollDelta.y > 0f) { activeModule++; }
        else if (Input.mouseScrollDelta.y < 0f) { activeModule--; }
        
        //keep active module in valid range so we don't switch to non-existant modules
        if (activeModule > modules.Count - 1 ) { activeModule = 0; }
        else if (activeModule < 0) { activeModule = modules.Count - 1; }

        //check if we are switching modules
        if (currentModule != modules[activeModule])
        {       
            currentModule.gameObject.transform.Find("Sprite").GetComponent<SpriteRenderer>().enabled = false;       //disable sprite for current module
            currentModule.GetComponent<Module>().shooting = false;                                                  //stop shooting
            currentModule = modules[activeModule];                                                                  //set the new current module
            currentModule.gameObject.transform.Find("Sprite").GetComponent<SpriteRenderer>().enabled = true;        //enable sprite for the new current module

            //check what module we are using and change our sprite accordingly
            switch(currentModule.name)
            {
                case "Gun Module":
                    weaponSprite.GetComponent<Image>().sprite = gunSprite;                                          //change our sprite
                    break;

                case "Burst Module":
                    weaponSprite.GetComponent<Image>().sprite = spreadSprite;
                    break;

                case "Rocket Module":
                    weaponSprite.GetComponent<Image>().sprite = rocketSprite;
                    break;

                case "Shield Module":
                    weaponSprite.GetComponent<Image>().sprite = shieldSprite;
                    break;
            }
        }
    }

    //This function is called by Unity whenever a trigger type object is collided with the player
    //The collision variable contains a reference to the object the player collided with
    private void OnTriggerEnter2D(Collider2D collision)
    {
        //check if we collided with a module and if that module was not attatched to the boss
        if (collision.gameObject.tag == "Player Module" && !collision.gameObject.GetComponent<Module>().isAttachedToBoss)
        {
            tempString = collision.gameObject.name.Substring(0, collision.gameObject.name.Length - 7);              //set our temporary string to the collided modules name
            
            //loop over all our modules and check if we already have the same module, break out of loop if we do
            for (int counter = 0; counter < modules.Count; counter++)
            {
                if (modules[counter].name == tempString)
                {
                    haveCheck = true;                       
                    counter = modules.Count + 1;
                }
            }

            //if we dont already have this module, add it to the player
            if (!haveCheck)
            {
                modules.Add(gameObject.transform.Find(tempString).gameObject);
                Debug.Log("picked up " + tempString);
            }

            //reset some variables for the next collision
            tempString = null;
            haveCheck = false;
        }
    }
}
