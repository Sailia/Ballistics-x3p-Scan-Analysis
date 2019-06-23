using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GPU_GraphicsBlitPerformance : MonoBehaviour
{
	public Material mat; //Material defined

    // Start is called before the first frame update
    void Start()
    {
        
    }

	private void OnRenderImage(RenderTexture source, RenderTexture destination)
	{

		// Src is the full rendered scene that you would normally
		// send directly to the monitor. We are intercepting
		// this so we can do a bit more work, before passing it on


		Graphics.Blit(source, destination, mat); //pass material into Graphics.Blit to process the material before sending to destination
	}


	// Update is called once per frame
	void Update()
    {
        
    }
}
