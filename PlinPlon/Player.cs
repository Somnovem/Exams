using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{  
    [SerializeField] public float movementSpeed = 6f;
    [SerializeField] public float maxVelocity = 7f;
    [SerializeField] public int maxDashes = 3;
    [SerializeField] public float dashRestoreTime = 5f;

    private Rigidbody2D rb;
    private int avaliableDashes;
    private float dashTimer;

    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
        avaliableDashes = maxDashes;
        dashTimer = Time.time;
    }

    // Update is called once per frame
    void Update()
    {
        float moveHorizontal = Input.GetAxis("Horizontal");
        float moveVertical = Input.GetAxis("Vertical");

        
        Vector2 movement = new Vector2(moveHorizontal * movementSpeed, moveVertical * movementSpeed);
        rb.AddForce(movement, ForceMode2D.Force);

        
        if (rb.velocity.magnitude > 0f)
        {
            Vector2 friction = -rb.velocity.normalized * Mathf.Sqrt(rb.velocity.magnitude) * 2f;
            rb.AddForce(friction, ForceMode2D.Force);
        }

        if(Time.time - dashTimer >= dashRestoreTime)
        {
            if(avaliableDashes < maxDashes)
            {
                avaliableDashes++;
            }
            dashTimer = Time.time;
        }

        if(Input.GetKeyDown(KeyCode.LeftShift) && avaliableDashes > 0)
        {
            rb.AddForce(movement * 5, ForceMode2D.Impulse);
            avaliableDashes--;
        }
    }
}
