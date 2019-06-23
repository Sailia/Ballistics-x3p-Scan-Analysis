using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CPU_GraphicsBlitPerformance : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

	private void OnRenderImage(RenderTexture source, RenderTexture destination)
	{
        // Src is the full rendered scene that you would normally
        // send directly to the monitor. We are intercepting
        // this so we can do a bit more work, before passing it on

        //pretend to do an image effect in CPU
        Color[] pixels = new Color[1920 * 1080];

        for(int x = 0; x < 1920; x++)
        {
            for(int y = 0; y < 1080; y++)
            {
                pixels[x + y * 1080].r = Mathf.Pow(2.18f, 3.17f);
            }
        }
        
		Graphics.Blit(source, destination);


	}


	// Update is called once per frame
	void Update()
    {
        
    }
}
