using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BallMovement : MonoBehaviour
{
    private Rigidbody2D rb;
    private Renderer objectRenderer;
    private Color maxColor;
    private Color midTopColor;
    private Color midBottomColor;
    private float currentSpeed;

    [SerializeField]public float maxSpeed = 6f;

    // Start is called before the first frame update
    void Start()
    {
        maxColor = new Color(255, 51, 51);
        midTopColor = new Color(255, 153, 153);
        midBottomColor = new Color(255, 204, 204);
        rb = GetComponent<Rigidbody2D>();
        objectRenderer = GetComponent<Renderer>();
    }

    void Update()
    {
        currentSpeed = rb.velocity.magnitude;
        if(currentSpeed > maxSpeed)
        {
            rb.velocity = rb.velocity.normalized * maxSpeed;
            currentSpeed = rb.velocity.magnitude;
        }
        if(currentSpeed > 5f)
        {
            objectRenderer.material.SetColor("_Color",maxColor);
        }
        else if(currentSpeed > 2.5f)
        {
            objectRenderer.material.SetColor("_Color",midTopColor);
        }
        else if(currentSpeed > 1f)
        {
            objectRenderer.material.SetColor("_Color",midBottomColor);
        }
        else
        {
            objectRenderer.material.SetColor("_Color",Color.white);
        }
    }
}
