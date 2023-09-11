Bun.serve({
  port: 8080,
  fetch: (req: Request) => {
    return new Response(
      JSON.stringify({
        message:
          "We've been trying to reach you about your car's extended warranty",
      })
    );
  },
});
